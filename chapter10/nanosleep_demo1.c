/*****************************************************************************
  Title       : nanosleep_demo1.c
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
#include "time_utils.h"

void sigint_handler( int signum)
{
    return;
}

int main(int argc, char* argv[])
{
    struct timespec initial_sleep, remainder;
    char            errmssge[100];
    int             retval;
    double          delay = 5; /* Default delay */
    struct sigaction act;

    if ( argc >= 2) {
        retval = get_dbl(argv[1], NON_NEG_ONLY|PURE, &delay, errmssge );
        if ( retval < 0 )
            fatal_error(retval, errmssge);
        else if (delay <= 0 )
            fatal_error(retval, "get_dbl requires a positive number"
                                " without trailing characters.\n");
    }
    /* Convert input to a timespec. */
    dbl_to_timespec(delay, &initial_sleep);

    /* Set up and install SIGINT handler. */
    act.sa_handler = sigint_handler;
    if ( -1 == sigaction(SIGINT, &act, NULL) )
        fatal_error(errno, "sigaction");

    printf("About to sleep for %10.10f seconds...\n", delay);
    if ( (-1 == nanosleep(&initial_sleep, &remainder)) && (errno == EINTR)) {
        /* Sleep was interrupted by a handled signal (SIGINT). */
        timespec_to_dbl(remainder, &delay); /* Convert remaining time. */
        printf("nanosleep() had %10.10f seconds left when it was "
               "interrupted.\n", delay);
    }
    return 0;
}

