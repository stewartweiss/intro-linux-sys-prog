/*****************************************************************************
  Title       : nanosleep_demo3.c
  Author      : Stewart Weiss
  Created on  : January 25, 2024
  Description : Display a blinking shape during process sleep
  Purpose     : Shows one way to create flashing cursor
  Usage       : nanosleep_demo3 [seconds]
                   If seconds is not supplied, the default is 5 seconds
  Build with  : gcc -Wall -g -I../include -L../lib -o nanosleep_demo3  \
                  nanosleep_demo3.c -lspl

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

#define NSECS  250000000L    /* 250,000,000 nsecs = 1/4 second */

int main(int argc, char* argv[])
{
    struct timespec sleeptime = {0,NSECS};
    int i, retval;
    int delay = 5;

    if ( argc >= 2) {
        retval = get_int(argv[1], NON_NEG_ONLY|PURE, &delay, NULL );
        if ( retval < 0 || delay < 1 )
            fatal_error(retval, "get_int expects a positive integer");
    }
    printf("Delaying for %d seconds...\n", delay);

    /* The loop iterates M times, where M = delay*(number of nanosleep
       intervals. */
    for ( i = 0; i < delay*(1000000000/NSECS); i++) {
        nanosleep(&sleeptime, NULL);
        if ( i & 1 ) printf("\r:-)");
        else         printf("\r:-(");
        fflush(stdout);
    }
    printf("Done.\n");
    return 0;
}
