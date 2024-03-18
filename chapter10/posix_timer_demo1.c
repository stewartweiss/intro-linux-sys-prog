/*****************************************************************************
  Title       : posix_timer_demo1.c
  Author      : Stewart Weiss
  Created on  : December 19, 2023
  Description : Sleep for real-valued number of seconds
  Purpose     : Show how nanosleep() behaves
  Usage       : nanosleep_demo [seconds]
                   If seconds is not supplied, the default is 5 seconds
                   Can be real-value, e.g. 12.345
  Build with  : gcc -Wall -g -I../include -L../lib -o nanosleep_demo1  \
                  nanosleep_demo1.c -lspl
  Notes:
  After starting the program, enter CTRL-C. The program will display
  how much time was left in the call to nanosleep().
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
#include "time_utils.h"

#define CLOCKID CLOCK_MONOTONIC
#define NUMTIMERS 8

char* idstr[] =
    {" 2", " 3", " 5", " 7", " 11", " 13", " 17 ", " 19 "};


void sighandler(int sig, siginfo_t *si, void *uc)
{
    long  timerid = *(long*) (si->si_value.sival_ptr);
    write(STDOUT_FILENO, idstr[timerid], strlen(idstr[timerid]));
    fflush(stdout);
}

int  main(int argc, char *argv[])
{
    timer_t timerid[NUMTIMERS];
    struct sigevent sev;
    struct itimerspec timer_setting[NUMTIMERS];
    struct sigaction sa;
    char c;
    int i, nbytes;
    int numtimers = NUMTIMERS;
    const double  BASE_UNIT = 0.4; /* Seconds */
    int interval[NUMTIMERS] = {2, 3, 5, 7, 11, 13, 17, 19};


    if ( SIGRTMIN+NUMTIMERS > SIGRTMAX )
        numtimers = SIGRTMAX - SIGRTMIN;

    /* Install signal handler. */
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    for ( i = 0; i < numtimers; i++ ) {
        sa.sa_sigaction = sighandler;
        sigfillset(&sa.sa_mask);
        if (-1 == sigaction(SIGRTMIN+1+i, &sa, NULL) )
            fatal_error(errno, "sigaction");
    }

    /* Create the timers. */
    sev.sigev_notify = SIGEV_SIGNAL;
    for ( i = 0; i < numtimers; i++ ) {
        sev.sigev_signo = SIGRTMIN+1+i;
        sev.sigev_value.sival_ptr = &(timerid[i]);
        if (timer_create(CLOCKID, &sev, &(timerid[i])) == -1)
           fatal_error(errno, "timer_create");
    }

    for ( i = 0; i < numtimers; i++ ) {
        /* Set the intervals for the timers. */
        dbl_to_timespec(interval[i]*BASE_UNIT,&(timer_setting[i].it_value));
        timer_setting[i].it_interval = timer_setting[i].it_value;
    }

    for ( i = 0; i < numtimers; i++ )
        if (timer_settime(timerid[i], 0, &(timer_setting[i]), NULL) == -1)
            fatal_error(errno, "timer_settime");

    printf("Enter 'q' to terminate this program.\n");
    while ( TRUE ) {
        if ( -1 == (nbytes = read(STDIN_FILENO, &c, 1)) )
            fatal_error(errno, "read");
        else if ( c == 'q' )
            break;
    }
    exit(EXIT_SUCCESS);
}

