#ifndef __SHOW_TIME_H__
#define __SHOW_TIME_H__

/*****************************************************************************
  Title          : show_time.h
  Author         : Stewart Weiss
  Created on     : February  9, 2010
  Description    : Header file for show_time.c
  Purpose        : Displays date in format "Feb  4 00:46:40 EST 1991"
  Modified on    : March 1, 2011
                   added functions time_plus() and time_minus()

******************************************************************************
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


/******************************************************************************
  Convert the time value into a date in the format
        "Feb  4 00:46:40 EST 1991"
******************************************************************************/
char* get_date( time_t timeval );



/******************************************************************************
  Convert the given time value into a date in the format
        "Feb  4 00:46:40 EST 1991"

  and display on the standard output.
******************************************************************************/
void show_time( time_t timeval );


/******************************************************************************
  Given a time_t now, returns a new time_t by adding the days
  hours, minutes, and seconds specified
******************************************************************************/
time_t time_plus( time_t now, int days, int hours, int mins, int secs);

/******************************************************************************
  Given a time_t now, returns a new time_t by subtracting the days
  hours, minutes, and seconds specified
******************************************************************************/
time_t time_minus( time_t now, int days, int hours, int mins, int secs);



#endif /* __SHOW_TIME_H__ */


