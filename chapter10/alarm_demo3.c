/*****************************************************************************
  Title       : alarm_demo3.c
  Author      : Stewart Weiss
  Created on  : December 19, 2023
  Description : Calls alarm, pause using the sigaction() to install handler
  Purpose     :
  Usage       : alarmdemo3  <numseconds>
                 where <numseconds> is the number of seconds with which to
                       set the alarm
  Build with  : gcc -Wall -g -I../include -L../lib -o alarm_demo3  \
                  alarm_demo3.c -lspl

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



/* This signal handler catches the SIGALRM and prints its delivery time.    */
void catchalarm(int signo )
{
    time_t t;
    struct tm *tp;
    char   timestr[100];
    t = time(NULL);
    if ( NULL == (tp = localtime(&t)))
        fatal_error(-1, "localtime");
    strftime(timestr, sizeof(timestr), "Alarm received at %T\n", tp);
    printf("%s", timestr );
}

int main(int argc, char * argv[])
{
    int    k;
    struct tm *bdtime;
    time_t t;
    struct sigaction act;
    char   timestr[100];
    int    resultcode;

    if (2 > argc) {
        usage_error("alarm_demo3 <alarm-interval>");
    }

    resultcode = get_int(argv[1], NON_NEG_ONLY|PURE, &k, NULL );
    if ( resultcode < 0 || k < 1 )
        fatal_error(resultcode, "get_int expects a positive integer");

    act.sa_handler = catchalarm;    /* install catchalarm as handler */
    sigfillset (&(act.sa_mask));    /* create a full mask */
    sigaction(SIGALRM, &act, NULL); /* catchalarm handles SIGALRM */

    time (&t);
    if ( (bdtime = localtime(&t)) == NULL )
        fatal_error(EOVERFLOW, "localtime");

    if (0 == strftime(timestr, sizeof(timestr), "The time is now %T", bdtime))
        fatal_error(BAD_FORMAT_ERROR,
                     "Conversion to a date-time string failed\n");
    printf( "%s. About to set an alarm for %d seconds.\n", timestr, k);
    alarm(k);
    pause();
    return 0;
}
