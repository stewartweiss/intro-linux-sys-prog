/*****************************************************************************
  Title       : alarm_demo1.c
  Author      : Stewart Weiss
  Created on  : December 19, 2023
  Description : Calls alarm, pause using the sigaction() to install handler
  Purpose     :
  Usage       : alarmdemo2  <numseconds>
                 where <numseconds> is the number of seconds with which to
                       set the alarm
  Build with  : gcc -Wall -g -I../include -L../lib -o alarm_demo1  \
                  alarm_demo1.c -lspl

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


int main(int argc, char * argv[])
{
    int    k, resultcode;

    if (2 > argc) {
        usage_error("alarm_demo1 <alarm-interval>");
    }

    resultcode = get_int(argv[1], NON_NEG_ONLY|PURE, &k, NULL );
    if ( resultcode < 0 || k < 1 )
        fatal_error(resultcode, "get_int expects a positive integer");

    printf( "Sleeping for %d seconds.\n",  k);
    alarm(k);
    pause();
    return 0;
}
