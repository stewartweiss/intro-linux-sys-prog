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
#include <math.h>

/* A macro that fills a timespec struct with the time from a double t */
#define  TIME2TS(t,ts)  (ts)->tv_sec = floor((t)); \
          (ts)->tv_nsec = 1000000000 * ((t) - (ts)->tv_sec);

/** dbl_to_timespec()
    Converts the time represented by the timespec ts to a double-precision
    float.
 *  @param  double            t   [IN]   number to be converted to a timespec
 *  @param  struct timespec  *ts  [OUT]  timespec representation of t
 *  @return -1  if t is negative, else 0
 */
int dbl_to_timespec( double t, struct timespec *ts);


/** add_dbl_to_timespec(t, &ts, &newtime)
    Sums the number of seconds represented by t and the  timespec ts into
    a timespec newtime.
 *  @param  double            t      [IN] number to be converted to a timespec
 *  @param  struct timespec  *ts     [IN] timespec representation of t
 *  @param  struct timespec *newtime [OUT] sum of t and ts
 *  @return -1  if t is negative, else 0
 */
void add_dbl_to_timespec( double t, struct timespec *ts,
                          struct timespec *newtime);


/** timespec_to_dbl()
    Converts the time represented by the timespec ts to a double-precision
    float.
 *  @param  struct timespec  ts   [IN]  timespec to convert
 *  @param  double           *t   [OUT]  double representing timespec's time
 */
void timespec_to_dbl( struct timespec ts, double  *t);


/** timespec_diff(ts1, ts2, &diff)
    Stores the difference  ts1 - ts2 into &diff.
 *  @param  struct timespec  ts1    [IN]
 *  @param  struct timespec  ts2    [IN]
 *  @param  struct timespec *diff   [OUT] ts1 - ts2
 */
void timespec_diff ( struct timespec ts1, struct timespec ts2,
                   struct timespec *diff );

/** timespec_add(ts1, ts2, &sum)
    Stores the sum of the times of timespecs ts1 and ts2 into &sum.
 *  @param  struct timespec  ts1    [IN]
 *  @param  struct timespec  ts2    [IN]
 *  @param  struct timespec *diff   [OUT] ts2 + ts1
 */
void timespec_add ( struct timespec ts1, struct timespec ts2,
                    struct timespec *sum );

#endif /* TIME_UTILS_H */
