/*****************************************************************************
  Title       : clock_nanosleep_demo.c
  Author      : Stewart Weiss
  Created on  : January 25, 2024
  Description : Acts like a call to sleep(), but with ns resolution
  Purpose     : Show how clock_nanosleep_demo() behaves
  Usage       : clock_nanosleep_demo[seconds]
                   If seconds is not supplied, the default is 5 seconds
  Build with  : gcc -Wall -g -I../include -L../lib -o clock_nanosleep_demo  \
                  clock_nanosleep_demo.c -lspl -lm

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

const char mssge[]="Signal received.\n";

void handler( int signum)
{
    write(STDOUT_FILENO, mssge, strlen(mssge));
}

int main(int argc, char* argv[])
{
    struct timespec endsleep, starttime, endtime, difftime;
    char            errmssge[100];
    int             retval;
    double          delay = 5; /* Default delay */
    struct sigaction act;
    double          total;

    if ( argc >= 2) {
        retval = get_dbl(argv[1], NON_NEG_ONLY|PURE, &delay, errmssge );
        if ( retval < 0 )
            fatal_error(retval, errmssge);
        else if ( delay <= 0 )
            fatal_error(retval, "get_dbl requires a positive number"
                                " without trailing characters.\n");
    }

    act.sa_handler = handler;
    if ( -1 == sigaction(SIGINT, &act, NULL) )
        fatal_error(errno, "sigaction");

    printf("Delaying for %10.10f seconds...\n", delay);
    if ( -1 == clock_gettime(CLOCK_MONOTONIC, &starttime))
        fatal_error(errno, "clock_gettime");

    add_dbl_to_timespec(delay, &starttime, &endsleep);

    do {
        retval = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,
                                 &endsleep, NULL);
        if (retval != EINTR && retval > 0 )
                fatal_error(errno, "clock_nanosleep");
    } while ( retval != 0 ); /* Repeat until retval is zero. */

    /* Get time now. */
    if ( -1 == clock_gettime(CLOCK_MONOTONIC, &endtime))
        fatal_error(errno, "clock_gettime");
    /* Compute time difference from start to finish of sleep. */
    timespec_diff(endtime, starttime, &difftime);

    /* Convert to a double. */
    timespec_to_dbl(difftime, &total);
    printf("Sleep lasted %10.10f seconds, "
           "%10.10f seconds longer than requested.\n", total, total - delay);
    return 0;
}
