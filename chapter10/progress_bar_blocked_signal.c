/*****************************************************************************
  Title          : progress_bar_blocked_signal.c
  Author         : Stewart Weiss
  Created on     : 1/23/2024
  Description/   : Displays a progress bar for a simulated computation
  Purpose        : To show how a POSIX timer can be used as an interval timer
  Usage          : progress_bar2
  Build with     : gcc -Wall -g -I../include -L../lib \
                    -o progress_bar_blocked_signal  \
                       progress_bar_blocked_signal.c -lspl

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
 #include <stdint.h>

#define MIN_SIMULATION_SECS  16      /* Minimum simulation time (seconds)   */
#define BAR_LENGTH           64      /* Length of progress bar between [ ]  */
#define DONE_CHAR           '#'      /* Character for completed part        */
#define NOT_DONE_CHAR       '-'      /* Character for incomplete part       */
#define SLEEPNSECS    480000000      /* Nanosecs in simulated differential  */

double  fraction_completed;           /* Fraction of operation completed    */
int or;

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
    memset(initial_bar, '-', BAR_LENGTH);
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
    sigaddset(&blocked_signals, SIGUSR1);
    while ( fraction_completed < 1.0 ) {
        sigprocmask(SIG_BLOCK, &blocked_signals, NULL);
        if ( -1 == nanosleep(&differential, &rem) )
            nanosleep(&rem, NULL );
        sigprocmask(SIG_UNBLOCK, &blocked_signals, NULL);

        sigprocmask(SIG_BLOCK, &blocked_signals, NULL);
        fraction_completed += progress_rate * drand48();
        if ( fraction_completed > 1.0 )
            fraction_completed = 1.0;
        sigprocmask(SIG_UNBLOCK, &blocked_signals, NULL);
    }

}


/* Signal handler for SIGUSR1  */
void refresh_progressbar( int signum, siginfo_t *si, void *uc)
{
    char bar[BAR_LENGTH+1];

    int finished_work = (int) (fraction_completed * BAR_LENGTH);
    bar[BAR_LENGTH] = '\0';
    memset(bar, NOT_DONE_CHAR, BAR_LENGTH);
    memset(bar, DONE_CHAR, finished_work);
    printf("\r[%s] %%%d", bar, (int)(100*fraction_completed));
    fflush(stdout);

}

void sig_handler( int signum )
{
    erase_progress_bar();
    raise(SIGTERM);
}

int main( int argc, char* argv[])
{
    struct sigaction act;
    const struct timespec zero_interval = {0,0};
    struct timespec slight_pause;
    struct timespec rem, ts_period;

    timer_t timerid;
    struct itimerspec interval;
    struct sigevent sev;
    int    retval;
    char   errmssge[100];
    double period = 0.5;

    /* Initialize the sigevent structure to send a SIGUSR1 signal on timer
       expiration. */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    sev.sigev_value.sival_ptr = &timerid;

   /* Create a timer with signal notification method  SIGUSR1.  */
    if (-1 == timer_create(CLOCK_MONOTONIC, &sev, &timerid) )
       fatal_error(errno, "timer_create");

    if ( argc >= 2) {
        retval = get_dbl(argv[1], NON_NEG_ONLY|PURE, &period, errmssge );
        if ( retval < 0 )
            fatal_error(retval, errmssge);
        else if (period <= 0 )
            fatal_error(retval, "get_dbl requires a positive number"
                                " without trailing characters.\n");
    }
    double maxval = (double) MIN_SIMULATION_SECS / 2.0 ;
    /* Check that the user chosen interval is not too large. */
    if ( period > maxval  ) {
        sprintf(errmssge,"Argument should be at most %4.1f", maxval);
        usage_error(errmssge);
    }
    /* Convert input to a timespec. */
    dbl_to_timespec(period, &ts_period);

    /* Compute the maximum of the ts_period and 3 seconds. */
    if ( period > 3.0 )
        slight_pause = ts_period;
    else {
        slight_pause.tv_sec = 3;
        slight_pause.tv_nsec = 0;
    }

    interval.it_value = ts_period;
    interval.it_interval = ts_period;


    draw_initial_bar();

    act.sa_handler = sig_handler;
    if ( sigaction(SIGINT, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    if ( sigaction(SIGQUIT, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    /* Install update_progressbar as the SIGUSR1 handler */
    act.sa_sigaction = refresh_progressbar;
    if ( sigaction(SIGUSR1, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    fraction_completed = 0;
    if (-1 == timer_settime(timerid, 0, &interval, NULL) )
        fatal_error(errno, "timer_settime");
    long_running_task();

    printf("Timer overruns: %d\n", timer_getoverrun(timerid));
    if ( -1 == nanosleep(&slight_pause, &rem) )  /* Pause before erasing progress bar. */
        nanosleep(&rem, NULL);
    interval.it_value = zero_interval;
    if ( -1 == timer_settime(timerid, 0, &interval, NULL) )
        fatal_error(errno, "timer_settime");

    act.sa_handler = SIG_IGN;
    if ( sigaction(SIGUSR1, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    erase_progress_bar();
    printf("Done\n");
    return 0;
}
