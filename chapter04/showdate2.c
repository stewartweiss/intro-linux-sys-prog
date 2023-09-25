/****************************************************************************
  Title          : showdate2.c
  Author         : Stewart Weiss
  Created on     : March 11, 2023
  Description    : Displays the current date/time, with format option +FORMAT
  Purpose        : To work with dates and times
  Usage          : showdate2
  Build with     : gcc -Wall -g -L../lib -o showdate2 showdate2.c -lspl
  Modifications  :

*****************************************************************************

Copyright (C) 2023 - Stewart Weiss

This code is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

****************************************************************************/
#define _GNU_SOURCE
#include "../include/common_hdrs.h"
#include "../include/common_defs.h"

#define FORMAT  "%c"
#define MAXLEN  STRING_MAX        /* Maximum size of message string */
#define BAD_FORMAT_ERROR    -1


int  main(int argc, char *argv[])
{
    char       formatted_date[MAXLEN];
    time_t     current_time;
    struct tm *broken_down_time;
    char       format_string[MAXLEN];
    char       err_msg[MAXLEN]; /* For error messages        */

    if ( argc < 2 )  /* No argument - use default. */
        strcpy(format_string, FORMAT);
    else {
        if ( argv[1][0] == '+' ) /* argument starts with + */
            if ( strlen(argv[1]+1) < MAXLEN )
                strncpy(format_string, argv[1]+1, MAXLEN-1);
            else {
                sprintf(err_msg, "format string length is too long\n");
                fatal_error(BAD_FORMAT_ERROR, err_msg);
            }
        else {
            sprintf(err_msg,"%s: format should be +\"format-string\"\n",
                    basename(argv[0]));
            fatal_error(BAD_FORMAT_ERROR,err_msg);
        }
    }

    /* get the current time */
    current_time = time(NULL);

    /* Convert the current time into broken-down time. */
    broken_down_time = localtime(&current_time);

    /* The only possible error is EOVERFLOW, and localtime returns NULL
       if the error occurred. */
    if (broken_down_time == NULL) {
        fatal_error(EOVERFLOW, "localtime");
    }

    /* Create a string from the broken down time using the %c format. */
    if (0 == strftime(formatted_date, sizeof(formatted_date),
             format_string, broken_down_time) ) {
        /* strftime does not set errno. If return is 0, it is an error
           because we expect a non-zero number of bytes in the output
           string. */
        fatal_error(BAD_FORMAT_ERROR, "Conversion to a date-time string failed or produced "
                        " an empty string\n");

    }

    /* Print the formatted date to standard output. */
    printf("%s\n", formatted_date);

    return 0;
}
