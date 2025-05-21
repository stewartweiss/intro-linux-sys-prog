/*****************************************************************************
  Title          : threaded_progbar.c
  Author         : Stewart Weiss
  Created on     : 6/23/2024
  Description/   : Displays a progress bar for a simulated computation
  Purpose        : To introduce mutexes in a multi-threaded program
  Usage          : threaded_progbar
  Build with     : gcc -Wall -g -I../include -L../lib -DNUMTHREADS=<n>\
                   -o threaded_progbar  threaded_progbar.c -lspl \
                   -pthread -lrt -lm
                   where <n> is the number of threads to create
  Note:
  This simulates a computation by updating a fraction_completed variable.
  The progress bar will not display properly if the number of columns in the
  terminal window is reduced and then increased.

******************************************************************************
* Copyright (C) 2024 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/
#include "common_hdrs.h"
#include "time_utils.h"
#include <pthread.h>


#define MIN_SIMULATION_SECS  10      /* Minimum simulation time (seconds)   */
#define REFRESH_INTERVAL      1      /* Number of seconds between refreshes */
#define BAR_LENGTH           64      /* Length of progress bar between [ ]  */
#define DONE_CHAR           '#'      /* Character for completed part        */
#define NOT_DONE_CHAR       '-'      /* Character for incomplete part       */

#ifndef NUMTHREADS
#define NUMTHREADS            4
#endif

double  fraction_completed = 0;       /* Fraction of operation completed    */
pthread_mutex_t  frac_mutex = PTHREAD_MUTEX_INITIALIZER;  /* Shared mutex   */
double  computedby[NUMTHREADS];       /* Fraction computed by each thread   */
pthread_t  t[NUMTHREADS];             /* Thread IDs                         */

typedef struct _task_data {
    int fd;     /* File descriptor for log file */
    long id;    /* Program's ID for thread      */
} thread_data;

/* Remove the progress bar from the display device. */
void erase_progress_bar()
{
    char  blanks[BAR_LENGTH+10];
    memset(blanks, ' ',BAR_LENGTH+9);
    blanks[BAR_LENGTH+9] = '\0';
    printf("\r%s\r", blanks);
    fflush(stdout);
}

void draw_initial_bar()
{
    char  initial_bar[BAR_LENGTH+1];
    memset(initial_bar, NOT_DONE_CHAR, BAR_LENGTH);
    initial_bar[BAR_LENGTH] = '\0';
    printf("\r[%s]", initial_bar);
    fflush(stdout);
}


void* long_running_task(void* arg)
{
    double progress_rate = 1.0 / (MIN_SIMULATION_SECS * NUMTHREADS);
    struct timespec differential, rem;

    thread_data td = *(thread_data*) arg;  /* Extract thread data from arg. */
    char  str[5];
    sprintf(str, "%ld,", td.id);            /* Thread ID as a string         */

    while ( fraction_completed < 1.0 ) {
        pthread_mutex_lock(&frac_mutex);         /* Lock mutex. */
        double work = progress_rate * ( 1.0*random())/RAND_MAX;
        fraction_completed += work;              /* Update is race-free. */
        if ( fraction_completed > 1.0 ) {
            work -= (fraction_completed - 1.0);
            fraction_completed = 1.0;
        }
        write(td.fd, str, strlen(str));    /* Record in log file that I ran. */
        pthread_mutex_unlock(&frac_mutex);       /* Unlock mutex. */
        dbl_to_timespec(20*NUMTHREADS*work, &differential);
        if ( -1 == nanosleep(&differential, &rem)) /* Sleep proportionately. */
            nanosleep(&rem, NULL );
        computedby[ td.id] += work;         /* Record fraction comnputed.    */
    }
    pthread_exit(EXIT_SUCCESS);
}

void refresh_progressbar( int signum)
{
    char bar[BAR_LENGTH+1];

    int finished_work;
    finished_work  = (int) (fraction_completed * BAR_LENGTH);
    bar[BAR_LENGTH] = '\0';
    memset(bar, NOT_DONE_CHAR, BAR_LENGTH);
    memset(bar, DONE_CHAR, finished_work);

    printf("\r[%s] %%%d", bar, (int)(100*fraction_completed));

    fflush(stdout);
    alarm(REFRESH_INTERVAL);
}

void* sighandler( void * data)
{
    int  sig;
    sigset_t  mask = *(sigset_t*) data;
    while ( TRUE ) {
        if ( sigwait(&mask, &sig) != 0 )
              fatal_error(errno, "sigwait");
        switch (sig ) {
        case SIGALRM:
            refresh_progressbar(sig);
            break;
        default:
            erase_progress_bar();
            for ( int i = 0; i < NUMTHREADS; i++ )
                pthread_cancel(t[i]);
            exit(EXIT_FAILURE);
        }
    }
    return data;
}


int main( int argc, char *argv[])
{
    const struct timespec slight_pause = {2,0};
    struct timespec remaining_sleep;
    pthread_t  sig_thread;
    sigset_t   mask;
    int i;
    int fd;
    thread_data td[NUMTHREADS];

    if ( -1 == (fd = open("./taskorder",
               O_WRONLY | O_TRUNC | O_CREAT | O_APPEND, 0644)))
        fatal_error(errno, "open");

    draw_initial_bar(); /* Draw the progress bar. */
    memset(computedby, 0, NUMTHREADS*sizeof (double));


    /* Block likely asynchronous signals. */
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGABRT);
    sigaddset(&mask, SIGALRM);
    if ( -1 == sigprocmask(SIG_BLOCK, &mask, NULL) )
        fatal_error(errno, "sigprocmask");

    /* Create one thread to handle asynchronous terminating signals. */
    if ( 0 != pthread_create(&sig_thread, NULL, sighandler, (void*) &mask ) )
        fatal_error(errno, "pthread_create");

    alarm(REFRESH_INTERVAL);

    for ( long  j = 0; j < NUMTHREADS; j++) {
        td[j].fd = fd;
        td[j].id = j;
        if ( 0 != pthread_create(&t[j], NULL, long_running_task, (void*) &td[j]) )
            fatal_error(errno, "pthread_create");
    }
    for (  i = 0; i < NUMTHREADS; i++)
        pthread_join(t[i], NULL);

    close(fd);
     /* Delay a bit before erasing the progress bar. */
    if ( -1 == nanosleep(&slight_pause, &remaining_sleep) )
        nanosleep(&remaining_sleep, NULL);
    alarm(0);
    erase_progress_bar();
    printf("Thread#    Percent\n");
    for ( i = 0; i < NUMTHREADS; i++ )
        printf("%d          %f\n", i, 100*computedby[i]);
    printf("The file ./taskorder has the sequence of thread accesses.\n");
    exit(EXIT_SUCCESS);
}
