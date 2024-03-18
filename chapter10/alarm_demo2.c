/*****************************************************************************
  Title       : alarm_demo2.c
  Author      : Stewart Weiss
  Created on  : December 19, 2023
  Description : Put process to sleep for specified period.
  Purpose     : Show how alarm() behaves
  Usage       : alarm_demo2 [seconds]
                   If seconds is not supplied, the default is 5 seconds

  Build with  : gcc -Wall -g -I../include -L../lib -o alarm_demo2  \
                  alarm_demo2.c -lspl

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


#define DEFAULT_DELAY   (5)
char  MESSAGE[]= "Received a wake-up signal!\n";

/* The signal handler for SIGALRM */
void catchalarm(int signum)
{
    write(STDOUT_FILENO, MESSAGE, sizeof(MESSAGE));
    exit(EXIT_SUCCESS);      /* Exit the program */
}

int main(int argc, char* argv[])
{
    int delay = DEFAULT_DELAY;
    int retval = 0, k = 0;
    struct timespec sleeptime = {1,0};   /* 1 second */
    struct sigaction act;

    if ( argc >= 2) {
        retval = get_int(argv[1], NON_NEG_ONLY|PURE, &delay, NULL );
        if ( retval < 0 || delay < 1 )
            fatal_error(retval, "get_int expects a positive integer");
    }
    act.sa_handler = catchalarm;
    sigemptyset(&(act.sa_mask));
    sigaction(SIGALRM, &act, NULL);
    printf("About to sleep for %d seconds\n", delay);
    alarm(delay);	           /* turn on alarm */
    while ( TRUE ) {
        nanosleep(&sleeptime, NULL);
        printf("%d second(s) elapsed\n",++k);
    }
    return 0;
}
