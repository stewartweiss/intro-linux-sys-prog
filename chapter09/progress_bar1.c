/*****************************************************************************
  Title          : progress_bar1.c
  Author         : Stewart Weiss
  Created on     : 1/23/2024
  Description/   : Displays a progress bar for a simulated computation
  Purpose        : To show how alarm() can be used as an interval timer
  Usage          : progress_bar1
  Build with     : gcc -Wall -g -I../include -L../lib -o progress_bar1 \
                   progress_bar1.c -lspl

  Note:
  This simulates a computation by updating a fraction_completed variable.
  The progress bar will not display properly if the number of columns in the
  terminal window is reduced and then increased.

******************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/
#include "common_hdrs.h"
#include <signal.h>

#define MIN_SIMULATION_SECS  16      /* Minimum simulation time (seconds)   */
#define REFRESH_INTERVAL      1      /* Number of seconds between refreshes */
#define BAR_LENGTH           64      /* Length of progress bar between [ ]  */
#define DONE_CHAR           '#'      /* Character for completed part        */
#define NOT_DONE_CHAR       '-'      /* Character for incomplete part       */
#define SLEEPNSECS    480000000      /* Nanosecs in simulated differential  */

double  fraction_completed = 0;       /* Fraction of operation completed    */

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


void long_running_task()
{
    double sleep_secs    = (double) (1.0*SLEEPNSECS) / 1000000000 ;
    double progress_rate = sleep_secs / MIN_SIMULATION_SECS;
    struct timespec differential={0, SLEEPNSECS}, rem;
    sigset_t    blocked_signals;

    sigemptyset(&blocked_signals);
    sigaddset(&blocked_signals, SIGALRM);
    while ( fraction_completed < 1.0 ) {
        if ( -1 == nanosleep(&differential, &rem) )
            nanosleep(&rem, NULL );
        sigprocmask(SIG_BLOCK, &blocked_signals, NULL);
        fraction_completed += progress_rate * drand48();
        if ( fraction_completed > 1.0 )
            fraction_completed = 1.0;
        sigprocmask(SIG_UNBLOCK, &blocked_signals, NULL);
    }
}

void refresh_progressbar( int signum)
{
    char bar[BAR_LENGTH+1];

    int finished_work = (int) (fraction_completed * BAR_LENGTH);
    bar[BAR_LENGTH] = '\0';
    memset(bar, NOT_DONE_CHAR, BAR_LENGTH);
    memset(bar, DONE_CHAR, finished_work);
    printf("\r[%s] %%%d", bar, (int)(100*fraction_completed));
    fflush(stdout);
    alarm(REFRESH_INTERVAL);
}

void sig_handler( int signum )
{
    erase_progress_bar();
    raise(SIGTERM);
}

int main( int argc, char* argv[])
{
    struct sigaction act;
    const struct timespec slight_pause = {2,0};
    struct timespec remaining_sleep;


    draw_initial_bar(); /* Draw the progress bar. */

    act.sa_handler = sig_handler;
    sigemptyset(&(act.sa_mask));
    act.sa_flags = 0;
    if ( sigaction(SIGINT, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    if ( sigaction(SIGQUIT, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    /* Install refresh_progressbar as the SIGALRM handler */
    act.sa_handler = refresh_progressbar;
    if ( sigaction(SIGALRM, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");
    alarm(REFRESH_INTERVAL);
    long_running_task();

     /* Delay a bit before erasing the progress bar. */
    if ( -1 == nanosleep(&slight_pause, &remaining_sleep) )
        nanosleep(&remaining_sleep, NULL);
    alarm(0);
    erase_progress_bar();
    printf("Done\n");
    return 0;
}
