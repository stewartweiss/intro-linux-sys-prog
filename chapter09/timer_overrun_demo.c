/*****************************************************************************
  Title          : timer_overrun_demo.c
  Author         : Stewart Weiss
  Created on     : March 18, 2024
  Description    : Causes deferred timer expirations and prints overruns
  Purpose        : To show how timer overruns can be handled
  Usage          : timer_overrun_demo1  delay interval
                      where delay is seconds and interval is nanoseconds
  Build with     : gcc -Wall -g -o timer_overrun_demo -I../include -L ../lib \
                      timer_overrun_demo.c -lspl -lrt

  Note:
  If you make the interval too small, such as 1 usec, the counts will
  not be reliable because this program does not protect against race
  conditions in counting and signals may arrive too quickly. Try 2 usecs or
  longer.  The total of the overruns and the caught signals should be
  roughly equal to delay*(1/frequency-in-seconds) = 10^9 * delay/frequency

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
#include <stdint.h>

#define CLOCKID CLOCK_MONOTONIC
#define STOPSIG  SIGRTMIN+1
#define TIMERSIG SIGRTMIN

long int         signal_count  = 0;
int              overrun_count = 0;
int              stop = 0;
timer_t          timerid;
struct timespec  zero_interval = {0,0};
struct itimerspec off_timer = {{0,0},{0,0}};

void timerhandler(int sig, siginfo_t *si, void *uc)
{
    signal_count++;
    int temp;
    timer_t *tidp;

    tidp = si->si_value.sival_ptr;
    temp = timer_getoverrun(*tidp);
    if (temp == -1)
        fatal_error(-1,"timer_getoverrun");
    overrun_count += temp;
}


void stophandler(int sig, siginfo_t *si, void *uc)
{
    if (timer_settime(timerid, 0, &off_timer, NULL) == -1)
            fatal_error(errno, "timer_settime");
    signal(SIGRTMIN, SIG_IGN);
    signal(sig, SIG_IGN);
    stop = 1;
}


int  main(int argc, char *argv[])
{
    timer_t stoptimerid;
    struct sigevent sev;
    struct itimerspec timer_setting1, timer_setting2;
    struct sigaction sa;
    sigset_t  mask;
    int delay;
    long freq;
    char msg[512];

    if ( argc < 3) {
        sprintf(msg, "usage: %s delay (seconds) interval (nanoseconds)\n",
                      argv[0]);
        usage_error(msg);
    }

    if ( ( VALID_NUMBER  != get_int(argv[1], POS_ONLY, &delay, 0) ) ||
       ( VALID_NUMBER != get_long(argv[2], POS_ONLY, &freq, 0)) ) {
        sprintf(msg, "usage: %s delay (seconds) interval (nanoseconds)\n",
                      argv[0]);
        usage_error(msg);
    }

    /* Install signal handlers. */
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = timerhandler;
    if (-1 == sigaction(SIGRTMIN, &sa, NULL) )
        fatal_error(errno, "sigaction");

    sa.sa_sigaction = stophandler;
    if (-1 == sigaction(STOPSIG, &sa, NULL) )
        fatal_error(errno, "sigaction");

    /* Create a mask for blocking SIGRTMIN. */
    sigemptyset(&mask);
    sigaddset(&mask, TIMERSIG);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
        fatal_error(errno, "main");

    /* Create the first timer. */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = TIMERSIG;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCKID, &sev, &timerid) == -1)
       fatal_error(errno, "timer_create");

    /* Create the "stop" timer - the one that stops the first. */
    sev.sigev_signo = STOPSIG;
    sev.sigev_value.sival_ptr = &stoptimerid;
    if (timer_create(CLOCKID, &sev, &stoptimerid) == -1)
       fatal_error(errno, "timer_create");


    /* Set the intervals for the timers. */
    timer_setting1.it_value.tv_sec = freq / 1000000000;
    timer_setting1.it_value.tv_nsec = freq % 1000000000;
    timer_setting1.it_interval.tv_sec = timer_setting1.it_value.tv_sec;
    timer_setting1.it_interval.tv_nsec = timer_setting1.it_value.tv_nsec;

    /* Set another timer to stop generation of timer signals by timer. */
    timer_setting2.it_value.tv_sec = delay;
    timer_setting2.it_value.tv_nsec = 0;
    timer_setting2.it_interval.tv_sec = 0;
    timer_setting2.it_interval.tv_nsec = 0;

    if (timer_settime(timerid, 0, &timer_setting1, NULL) == -1)
            fatal_error(errno, "timer_settime");

    /* Unblock the signals */
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)
        fatal_error(errno, "main");

    if (timer_settime(stoptimerid, 0, &timer_setting2, NULL) == -1)
            fatal_error(errno, "timer_settime");

    /* Busy-wait until stop timer expires. */
    while ( stop == 0 ) {}
    printf("Count = %ld; timer overruns: %d\n", signal_count, overrun_count);
    exit(EXIT_SUCCESS);
}
