/*****************************************************************************
  Title          : time_utils.h
  Author         : Stewart Weiss
  Created on     :
  Description    : An assortment of time utility functions

  Modifications:
    Nov. 2, 2025 by SNW
    Added a new version of timespec_diff() named timespec_diff2()

******************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.lgplv3 for details.               *
*****************************************************************************/

#include "common_hdrs.h"
#include "time_utils.h"
#include <math.h>
#include <assert.h>


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

/* This functions is intended to be used when ts1 >= ts2 as times.
   If the difference is negative, the result is not correct.
   Use timespec_diff2() in case the ts1 represents a smaller time than ts2.
*/
void timespec_diff ( struct timespec ts1, struct timespec ts2,
                   struct timespec *diff )
{
   long temp;
   diff->tv_sec = ts1.tv_sec - ts2.tv_sec;
   temp  = ts1.tv_nsec - ts2.tv_nsec;
   assert(diff >= 0 ); /* Added by SNW 11/2/2025. */
   if ( temp < 0 ) {
       /* We need to borrow 1 second from tv_sec and add it to tv_nsec. */
       diff->tv_sec--;                     /* Borrow second. */
       diff->tv_nsec = 1000000000 + temp;  /* Add it here.   */
   }
   else
       diff->tv_nsec = temp;
}

/* Added by SNW 11/2/2025.
   This function computes the difference between two timespec values
   regardless of whether the first is smaller than the second. However,
   it stores the result in a timespec struct with possibly negative
   values. The documentation states that the tv_sec and tv_nsec fields
   are suppoesd to be non-negative  when the timespec struct is passed
   as an argument to any library function or system call that expects
   a timespec struct argument. This should be borne in mind when using this
   function.
*/
void timespec_diff2 ( struct timespec ts1, struct timespec ts2,
                   struct timespec *diff )
{
    long temp;
    diff->tv_sec = ts1.tv_sec - ts2.tv_sec;
    temp  = ts1.tv_nsec - ts2.tv_nsec;
    if ( diff->tv_sec > 0 )
        if ( temp < 0 ) {
            /* Borrow 1 second from tv_sec and add it to tv_nsec.   */
            diff->tv_sec--;                     /* Borrow second.   */
            diff->tv_nsec = temp + 1000000000;  /* Add it here.     */
        }
        else    /* No adjustment needed, since both are positive.   */
            diff->tv_nsec = temp;
    else if ( diff->tv_sec < 0 )
        if ( temp > 0 )  {
            /* Borrow -1 seconds from tv_sec and add to tv_nsec.    */
            diff->tv_sec++;                     /* Subtract -1 sec. */
            diff->tv_nsec = temp - 1000000000;  /* Add it here.     */
        }
        else
            diff->tv_nsec =   temp;  /* Add it here.   */
    else {  /* diff.tv_sec == 0 . */
        diff->tv_nsec = temp;
    }
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


