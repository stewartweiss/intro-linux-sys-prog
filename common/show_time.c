/******************************************************************************
  Title          : show_time.c
  Author         : Stewart Weiss
  Created on     : February  9, 2010
  Description    : A collection of time-related functions for UNIX
  Purpose        : For formatting timevals
  Usage          : see below
  Build with     : gcc -c utils.c

******************************************************************************/

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


#include <time.h>
#include <stdio.h>



/******************************************************************************
  Convert the time value into a date into either the format
        "Feb  4 00:46"
  or the format
         Feb  4 2012
  depending on how recent the time is. It trieds to use the locale setting
  first in case it is a recent file.
   NOTE:
   We could use instead
        char datestring[200];
        strftime(datestring, sizeof(datestring),
             nl_langinfo(D_T_FMT), tmp);
   which gives us a string in the format
   DDD MMM DD HH:MM:SS YYYY
   and then from this pick out the substrings we want. That is an alternative.

******************************************************************************/

char* get_date( time_t timeval )
{
    const int  sixmonths = 15724800; /* number of secs in 6 months */
    static char outstr[200];
    struct tm *tmp;
    time_t current_time = time(NULL);
    int    recent = 1;

    if ( ( current_time - timeval ) > sixmonths )
        recent = 0;

    tmp = localtime(&timeval);
    if (tmp == NULL) {
        perror("get_date_no_day: localtime");
    }

    if ( ! recent ) {
        strftime(outstr, sizeof(outstr), "%b %e  %Y", tmp);
        return outstr;
    }
    else if (strftime(outstr, sizeof(outstr), "%c", tmp) > 0)
        return outstr+4;
    else {
        printf("error with strftime\n");
        strftime(outstr, sizeof(outstr), "%b %e %H:%M", tmp);
        return outstr;
    }

}

/******************************************************************************
  Convert the given time value into a date in the format
        "Feb  4 00:46:40"
  and display on the standard output.
******************************************************************************/

void show_time( time_t timeval )
{
    printf("%12.12s", get_date(timeval ));
}


/******************************************************************************
  Given a time_t now, returns a new time_t by adding the days
  hours, minutes, and seconds specified
******************************************************************************/
time_t time_plus( time_t now, int days, int hours, int mins, int secs)
{
    struct tm time_tm;
    time_t    newtime;

    localtime_r(&now, &time_tm);
    time_tm.tm_sec   += secs;
    time_tm.tm_min   += mins;
    time_tm.tm_hour  += hours;
    time_tm.tm_mday  += days;

    newtime = mktime(&time_tm);
    return newtime;
}

/******************************************************************************
  Given a time_t now, returns a new time_t by subtracting the days
  hours, minutes, and seconds specified
******************************************************************************/
time_t time_minus( time_t now, int days, int hours, int mins, int secs);



