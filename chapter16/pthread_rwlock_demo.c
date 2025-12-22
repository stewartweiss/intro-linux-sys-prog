
/*****************************************************************************
  Title          : pthread_rwlock_demo.c
  Author         : Stewart Weiss
  Created on     : Oct. 20, 2024
  Description    : Runs a set of readers and writers
  Purpose        : To show how to use reader/writer locks and how to avoid
                   starvation.
  Usage          : rwlock_demo [-r <nreaders> -w<nwriters> -s<sleeptime> \
                         -R ] <logile>
  Build with     : gcc -Wall -g -I../include -L../lib -o rwlock_demo \
                   rwlock_demo.c -lspl -pthread -lrt


  Notes:
  This program would be very simple if we didn't care about trying to prevent
  starvation, either of readers or writers. You can comment out the barrier
  code and the code to initialize the rwlock attributes in this case.

  The MONITOR flag turns on output that shows how many threads currently hold
  the lock. To do this we need a mutex to lock the counter so that the threads
  can increment and decrement it in mutual exclusion.

  The barrier is there so that the threads that are created first do not
  necessarily get the rwlock first; all threads start their loops at the same
  time (up to scheduling decisions).

  Play around with the command line options to see if you can make readers
  starve or writers starve. Do you see a pattern in the amount of time that
  readers and writers acquire the lock in terms of the numbers of readers
  and writers?

  Modifications:
                   12/20/2025 by SNW Changed type of ch in main to int

******************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

//#define MONITOR


#define  _GNU_SOURCE
#include "common_hdrs.h"
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

/*****************************************************************************
                             Data Types and Constants
******************************************************************************/

#define NUM_READERS   8    /* Default number of readers */
#define NUM_WRITERS   2    /* Default number of writers */
#define LINESIZE      64   /* Maximum length of string  */
#define MAX_ARRAYSIZE 1024 /* Maximum array size        */


/* The data structure representing a single item in the shared array. */
typedef struct _data {

    int             wrid;   /* Writer ID, negated        */
    struct timespec wrtime; /* Time of last modification */
    char  text[LINESIZE];   /* Actual text data          */
} item;


typedef struct _task_data
{
    int first;       /* Index of first element for thread */
    int last;        /* index of last element for thread  */
    int task_id;     /* Thread's program ID               */
    int num_threads; /* Total number of threads           */
    item *data;      /* Pointer to array of data          */
} reader_task_data;

item             *shared_data;         /* Dynamically-allocated data array  */
int               arraysize;           /* Actual size of the array          */
pthread_rwlock_t  rwlock;              /* The reader/writer lock            */
pthread_barrier_t barrier;             /* Barrier to improve fairness       */
pthread_mutex_t   print_mutex;         /* To serialize printing with printf */
FILE             *logfp;               /* FILE stream for log file          */
int              rdrsleep_ns = 200000; /* Nanosecs in reader delay time     */
#ifdef MONITOR
pthread_mutex_t   counter_mutex;       /* Used by the monitor code          */
int               num_threads_in_lock; /* for the monitor code */
#endif


void* sighandler( void * data)
{
    int  sig;
    sigset_t  mask = *(sigset_t*) data;

    if ( sigwait(&mask, &sig) != 0 )
          fatal_error(errno, "sigwait");
    exit(EXIT_SUCCESS);  /* Force all threads to exit. */

}

void *reader(void * data)
{
    int retval;
    reader_task_data *t_data  = (reader_task_data*) data;
    int t            = t_data->task_id;
    struct timespec sleeptime = {0,rdrsleep_ns};
    struct timespec rem = sleeptime;

    /* Wait here until all threads are created */
    retval = pthread_barrier_wait(&barrier);
    if ( PTHREAD_BARRIER_SERIAL_THREAD != retval &&  0 != retval )
        fatal_error( retval, "pthread_barrier_wait");

    while ( TRUE ) {
        if ( 0 != (retval = pthread_rwlock_rdlock(&rwlock)) )
            fatal_error( retval, "pthread_rwlock_rdlock");
        pthread_mutex_lock(&print_mutex);
        for ( int k = t_data->first; k <= t_data->last; k++ )
            printf("Read by %2d: %3d\t%6lu.%-12lu [%4d] %s", t,
                    t_data->data[k].wrid,
                    t_data->data[k].wrtime.tv_sec,
                    t_data->data[k].wrtime.tv_nsec,
                    k,
                    t_data->data[k].text);
        fflush(stdout);
        pthread_mutex_unlock(&print_mutex);
        fprintf(logfp, "Reader %d got the read lock\n", t);
        fflush(logfp);

#ifdef MONITOR
        pthread_mutex_lock(&counter_mutex);
        num_threads_in_lock++;
        pthread_mutex_unlock(&counter_mutex);
#endif
        if ( 0 != (retval = pthread_rwlock_unlock(&rwlock)) )
            fatal_error( retval, "pthread_rwlock_unlock");
        fprintf(logfp, "Reader %d released the read lock\n", t);
        fflush(logfp);

#ifdef MONITOR
        pthread_mutex_lock(&counter_mutex);
        num_threads_in_lock--;
        pthread_mutex_unlock(&counter_mutex);
#endif
        nanosleep(&sleeptime, &rem);
    }
    pthread_exit(NULL);
}

/** writer()
 *  A writer does the same thing as a reader -- it repeatedly gets the lock,
 *  sleeps a bit, and then releases the lock, until done becomes true.
 */
void *writer(void * data)
{
    int retval;                    /* Return values from function calls */
    int t = (int) (long) data;     /* Thread's program ID               */
    struct timespec dt   = {1, 0}; /* Sleep time for clock_nanosleep    */
    struct timespec curtime;       /* Time of modification              */

    /* Wait here until all threads are created */
    retval = pthread_barrier_wait(&barrier);
    if ( PTHREAD_BARRIER_SERIAL_THREAD != retval &&  0 != retval )
        fatal_error( retval, "pthread_barrier_wait");

    while ( TRUE ) {
        if ( 0 != (retval = pthread_rwlock_wrlock(&rwlock)) )
            fatal_error( retval, "pthread_rwlock_wrlock");
        clock_gettime(CLOCK_MONOTONIC, &curtime);
        for ( int i = 0; i < arraysize; i++ ) {
            if ( random() > RAND_MAX/2 ) {
                shared_data[i].wrid = -t;
                shared_data[i].wrtime = curtime;
            }
        }
        fprintf(logfp, "Writer %d got the write lock\n", t);
        fflush(logfp);
#ifdef MONITOR
        pthread_mutex_lock(&counter_mutex);
        num_threads_in_lock++;
        pthread_mutex_unlock(&counter_mutex);
#endif
        if ( 0 != (retval = pthread_rwlock_unlock(&rwlock)) )
            fatal_error( retval, "pthread_rwlock_unlock");
        fprintf(logfp, "Writer %d released the write lock\n", t);
        fflush(logfp);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &dt, NULL);

#ifdef MONITOR
        pthread_mutex_lock(&counter_mutex);
        num_threads_in_lock--;
        pthread_mutex_unlock(&counter_mutex);
#endif
    }
    pthread_exit(NULL);
}

/** monitor()
 * This periodically prints the number of threads holding the rwlock.
 */
#ifdef MONITOR
void *monitor(void * data)
{
    while ( TRUE ) {
        sleep(1);
        pthread_mutex_lock(&counter_mutex);
        fprintf(logfp, "Number of threads holding lock = %d\n",
                        num_threads_in_lock);
        fflush(stdout);
        pthread_mutex_unlock(&counter_mutex);
    }
    pthread_exit(NULL);
}
#endif

int  load_data(char *pathname, int maxsize, item *array)
{
    FILE *fp;
    int i = 0;
    char *buffer = NULL;
    size_t  len  = 0;

    if ( (fp = fopen(pathname, "r") ) == NULL )
        fatal_error(errno, "fopen");
    while ( (i < maxsize) && (-1 != getline(&buffer, &len, fp) )) {
        if ( len < LINESIZE )
            strncpy(array[i].text, buffer, len);
        else {
            strncpy(array[i].text, buffer, LINESIZE-1);
            array[i].text[LINESIZE-1] = '\0';
        }
        array[i].wrid = 0;
        array[i].wrtime.tv_sec = 0;
        array[i].wrtime.tv_nsec = 0;
        i++;
    }
    free(buffer);
    return i;
}


int main(int argc, char *argv[])
{
    int            retval;
    int            nreaders  = NUM_READERS;
    int            nwriters  = NUM_WRITERS;
    long int       t;
    unsigned int   num_threads;;
    pthread_t      *threads;
    pthread_t      sig_thread;      /* Thread ID for signal-handling thread */
    sigset_t       mask;            /* Signal mask of blocked signals       */
    pthread_attr_t attr;            /* Attribute structure for threads      */
    reader_task_data     *thread_data;
    BOOL           reader_preference = FALSE;
    int            ch;
    char           options[] = "r:w:s:R";


    opterr = 0;
    optind = 0;
    while  (TRUE) {
        ch = getopt(argc, argv, options);
        if ( -1 == ch )
            break;
        switch ( ch ) {
        case 'r':
            if ( VALID_NUMBER != get_int(optarg, NON_NEG_ONLY, &nreaders,
                 NULL ) )
                usage_error("Invalid argument to -r");
            break;
        case 'w':
            if ( VALID_NUMBER != get_int(optarg, NON_NEG_ONLY, &nwriters,
                 NULL ))
                usage_error("Invalid argument to -w");
            break;
         case 's':
            if ( VALID_NUMBER != get_int(optarg, NON_NEG_ONLY, &rdrsleep_ns,
                 NULL ))
                usage_error("Invalid argument to -s");
            if (rdrsleep_ns > 999999 )
                usage_error("Argument to -s must be at most 999999");
            break;
        case 'R':
            reader_preference = TRUE;
            break;
        }   }

    if ( optind >= argc )
        usage_error("rwlock_demo [-r <num> -w <num> -R ] data-file");


    num_threads         = nreaders + nwriters;
    arraysize           = MAX_ARRAYSIZE;

    /* Block likely asynchronous signals. */
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGABRT);
    if ( -1 == sigprocmask(SIG_BLOCK, &mask, NULL) )
        fatal_error(errno, "sigprocmask");

    /* Set the attribute structure to create a detached thread. */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize (&attr, 65536);

    /* Create one thread to handle asynchronous terminating signals. */
    if ( 0 != (retval = pthread_create(&sig_thread, &attr, sighandler,
                (void*) &mask ) ) )
        fatal_error(retval, "pthread_create");

    pthread_rwlockattr_t  rwlock_attributes;
    pthread_rwlockattr_init(&rwlock_attributes);
    /* The following non-portable function is a GNU extension that alters the
       thread priorities when readers and writers are both waiting on a rwlock,
       giving preference to writers.
    */
    if ( nwriters > 1 ) {
        if ( reader_preference )
            pthread_rwlockattr_setkind_np(&rwlock_attributes,
                   PTHREAD_RWLOCK_PREFER_READER_NP);
        else
            pthread_rwlockattr_setkind_np(&rwlock_attributes,
                   PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
    }
    /* Initialize the rwlock. */
    pthread_rwlock_init(&rwlock, &rwlock_attributes );

#ifdef MONITOR
    num_threads_in_lock = 0;
    pthread_mutex_init(&counter_mutex, NULL);
#endif
    pthread_mutex_init(&print_mutex, NULL);

    /* Allocate the shared array. */
    if ( NULL == (shared_data = (item*) calloc(arraysize, sizeof(item))) )
        fatal_error(errno, "calloc");
    arraysize = load_data(argv[optind], arraysize, shared_data);

    if ( (logfp = fopen("rwlockdemo.log", "w") ) == NULL )
        fatal_error(errno, "fopen");
    if ( NULL == (thread_data = calloc( nreaders, sizeof(reader_task_data))) )
        fatal_error(errno, "calloc");
    if ( NULL == (threads = calloc( num_threads, sizeof(pthread_t))) )
        fatal_error(errno, "calloc");

    /* Initialize a barrier with a count equal to the numebr of threads */
    if (0 != (retval = pthread_barrier_init(&barrier, NULL, num_threads)))
        fatal_error( retval, "pthread_barrier_init");

    for ( t = 0 ; t < nreaders; t++) {
        thread_data[t].first     = (t*arraysize)/nreaders;
        thread_data[t].last      = ((t+1)*arraysize)/nreaders -1;
        thread_data[t].task_id   = t;
        thread_data[t].num_threads = nreaders;
        thread_data[t].data      = shared_data;
    }
    for ( t = 0 ; t < nreaders; t++)
        if ( 0 != (retval = pthread_create(&threads[t], &attr, reader,
                             &thread_data[t])) )
            fatal_error(retval, " pthread_create");

    for ( t = nreaders ; t < num_threads; t++)
        if ( 0 != (retval = pthread_create(&threads[t], NULL, writer,
             (void *)t)) )
            fatal_error( retval, "pthread_create");

#ifdef MONITOR
    retval = pthread_create(&threads[t], NULL, monitor, NULL);
    if ( retval )
         fatal_error( retval, "pthread_create");
#endif
    for ( t = 0 ; t < num_threads; t++)
        pthread_join(threads[t],  NULL);

    exit(EXIT_SUCCESS);
}

