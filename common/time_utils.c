/*****************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.lgplv3 for details.               *
*****************************************************************************/

#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include "common_hdrs.h"
#include "time_utils.h"
#include <math.h>



int dbl_to_timespec( double t, struct timespec *ts)
{
    if ( t < 0 )
        return -1;
    ts->tv_sec = (time_t)  floor(t);
    ts->tv_nsec = 1000000000 * (t - ts->tv_sec);
    return 0;
}

void add_dbl_to_timespec( double t, struct timespec *ts,
                          struct timespec *newtime)
{
    long temp;
    double  seconds = floor(t);
    newtime->tv_sec = (time_t) (ts->tv_sec +  seconds );
    temp  = ts->tv_nsec + (long) (1000000000 * (t - seconds));
    if ( temp > 999999999 ) {
        newtime->tv_sec++;
        newtime->tv_nsec = temp - 1000000000;
    }
    else
        newtime->tv_nsec = temp;
    return ;
}



void timespec_to_dbl( struct timespec ts, double  *t)
{
    *t = ts.tv_sec + (double) ts.tv_nsec/1000000000;
}


void timespec_diff ( struct timespec ts1, struct timespec ts2,
                   struct timespec *diff )
{
   long temp;
   diff->tv_sec = ts1.tv_sec - ts2.tv_sec;
   temp  = ts1.tv_nsec - ts2.tv_nsec;
   if ( temp < 0 ) {
       /* We need to borrow 1 second from tv_sec and add it to tv_nsec. */
       diff->tv_sec--;                     /* Borrow second. */
       diff->tv_nsec = 1000000000 + temp;  /* Add it here.   */
   }
   else
       diff->tv_nsec = temp;
}

void timespec_add ( struct timespec ts1, struct timespec ts2,
                    struct timespec *sum )
{
   long temp;
   sum->tv_sec = ts1.tv_sec + ts2.tv_sec;
   temp  = ts1.tv_nsec + ts2.tv_nsec;
   if ( temp > 999999999 ) {
       /* We need to carry 1 second from tv_nsec and to tv_sec. */
       sum->tv_sec++;                     /* Carry second. */
       sum->tv_nsec = temp - 1000000000;  /* Subtract  it here.   */
   }
   else
       sum->tv_nsec = temp;
}

#endif /* TIME_UTILS_H */
