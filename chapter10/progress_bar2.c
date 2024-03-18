/*****************************************************************************
  Title          : progress_bar2.c
  Author         : Stewart Weiss
  Created on     : 1/23/2024
  Description/   : Displays a progress bar for a simulated computation
  Purpose        : To show how a POSIX timer can be used as an interval timer
  Usage          : progress_bar2
  Build with     : gcc -Wall -g -I../include -L../lib -o progress_bar2 \
                   progress_bar2.c -lspl

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
#include "time_utils.h"
#include "get_nums.h"


#define MIN_SIMULATION_SECS  12      /* Minimum simulation time (seconds)   */
#define BAR_LENGTH           64      /* Length of progress bar between [ ]  */
#define DONE_CHAR           '#'      /* Character for completed part        */
#define NOT_DONE_CHAR       '-'      /* Character for incomplete part       */
#define SLEEPNSECS    480000000      /* Nanosecs in simulated dt            */

double  fraction_completed =0;        /* Fraction of operation completed    */


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


void lengthy_task()
{
    double sleep_secs         = (double) (1.0*SLEEPNSECS) / 1000000000 ;
    double progress_rate      = sleep_secs / MIN_SIMULATION_SECS;
    struct timespec dt        = {0, SLEEPNSECS};
    sigset_t  blocked_signals;
    struct timespec endts, startts;
    int    ret;

    /* Create a signal set with just SIGUSR1. */
    sigemptyset(&blocked_signals);
    sigaddset(&blocked_signals, SIGUSR1);

    while ( fraction_completed < 1.0 ) {
        /* Set up the call to clock_nanosleep() by getting current time.    */
        if ( -1 == clock_gettime(CLOCK_MONOTONIC, &startts))
            fatal_error(errno, "clock_gettime");
        /* Add the dt interval to the current time to get
           absolute time at which timer should expire.                      */
        timespec_add ( dt, startts, &endts);

        /* Call clock_nanosleep() repeatedly until it returns 0.            */
        do {
            ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,
                                     &endts, NULL);
            if (ret != EINTR && ret > 0 )
                    fatal_error(errno, "clock_nanosleep");
        } while ( ret != 0 ); /* Repeat until retval is zero. */

        /* Block the SIGUSR1 signal during the update to fraction_completed. */
        sigprocmask(SIG_BLOCK, &blocked_signals, NULL);
        fraction_completed += progress_rate * drand48();
        if ( fraction_completed > 1.0 )
            fraction_completed = 1.0;
        sigprocmask(SIG_UNBLOCK, &blocked_signals, NULL);
    }
}


/* Signal handler for SIGUSR1  */
void refresh_progressbar( int signum)
{
    char bar[BAR_LENGTH+1];

    int finished_work = (int) (fraction_completed * BAR_LENGTH);
    bar[BAR_LENGTH] = '\0';
    memset(bar, NOT_DONE_CHAR, BAR_LENGTH);
    memset(bar, DONE_CHAR, finished_work);
    printf("\r[%s] %%%d", bar, (int)(100*fraction_completed));
    fflush(stdout);
}

void short_pause( double refresh_secs,
                           struct timespec refresh_timespec)
{
    struct timespec slight_pause;
    struct timespec remaining_sleep;

    if ( refresh_secs > 3.0 )
        slight_pause = refresh_timespec;
    else {
        slight_pause.tv_sec  = 3;
        slight_pause.tv_nsec = 0;
    }
    /* Sleep for slight_pause time. */
    if ( -1 == nanosleep(&slight_pause, &remaining_sleep) )
        nanosleep(&remaining_sleep, NULL);
}

void clean_up()
{
    struct sigaction act;

    act.sa_handler = SIG_IGN;
    if ( sigaction(SIGUSR1, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");
    erase_progress_bar();
    printf("Done\n");
}

void sig_handler( int signum )
{
    erase_progress_bar();
    raise(SIGTERM);
}

int main( int argc, char* argv[])
{
    struct sigaction act;                /* For calling sigaction           */
    int    retval;                       /* Return value from various calls */
    char   errmssge[100];                /* For printing errors             */
    double refresh_secs = 0.5;           /* Default refresh interval        */
    double max_interval;                 /* Max allowed refresh interval    */
    timer_t timerid;                     /* timer ID from timer_create()    */
    struct timespec zero_interval = {0,0};/* For zero-ing a timer value     */
    struct timespec refresh_timespec;    /* timespec for refresh interval   */
    struct itimerspec refresh_interval;  /* The timer value and repeat      */
    struct sigevent sev;                 /* Notification structure          */

    max_interval = (double) MIN_SIMULATION_SECS / 2.0 ;

    /* Check if command line has a refresh interval.  */
    if ( argc >= 2) {
        retval = get_dbl(argv[1], NON_NEG_ONLY, &refresh_secs, errmssge );
        if ( retval < 0 )
            fatal_error(retval, errmssge);
        else if (refresh_secs <= 0 )
            fatal_error(retval, "get_dbl requires a positive number.\n");
    }

    /* Check that the user chosen interval is not too large. */
    if ( refresh_secs > max_interval || refresh_secs < 0.001 ) {
        sprintf(errmssge,"Argument must be between  0.001 and %4.1f",
                          max_interval);
        usage_error(errmssge);
    }


    draw_initial_bar();
    /* Set up signal handling. */
    act.sa_handler = sig_handler;
    if ( sigaction(SIGINT, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");
    if ( sigaction(SIGQUIT, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    /* Install update_progressbar as the SIGUSR1 handler */
    act.sa_handler = refresh_progressbar;
    if ( sigaction(SIGUSR1, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    /* Initialize a sigevent structure to send a SIGUSR1 signal on timer
       expiration. */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo  = SIGUSR1;

    /* Create a timer with this sigevent structure.  */
    if (-1 == timer_create(CLOCK_MONOTONIC, &sev, &timerid) )
       fatal_error(errno, "timer_create");

    /* Convert refresh_secs length to a timespec. */
    dbl_to_timespec(refresh_secs, &refresh_timespec);

    /* Set up the timer's initial value and repeat interval to be the same. */
    refresh_interval.it_value    = refresh_timespec;
    refresh_interval.it_interval = refresh_timespec;

    /* Arm the timer just created with the itimerspec interval. */
    if (-1 == timer_settime(timerid, 0, &refresh_interval, NULL) )
        fatal_error(errno, "timer_settime");

    /* Start the simulated task. */
    lengthy_task();

    /* Time to clean up but first delay a bit.  */
    short_pause(refresh_secs, refresh_timespec);

    /* Zero the timer to disarm it. */
    refresh_interval.it_value = zero_interval;
    if ( -1 == timer_settime(timerid, 0, &refresh_interval, NULL) )
        fatal_error(errno, "timer_settime");

    clean_up();
    return 0;
}
