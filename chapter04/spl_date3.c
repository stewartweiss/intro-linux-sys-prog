/*****************************************************************************
  Title          : spl_date3.c
  Author         : Stewart Weiss
  Created on     : February  22, 2023
  Description    : Displays formatted time, possibly different from
                   current time
  Usage          : spl_date3 [-d<time-adjustment>] +"<format-specification>"
  Build with     : gcc -Wall -g -I../include -L../lib -o spl_date3 \
                    spl_date3.c -lspl

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

/* A format string is a sequence of one or more pairs of the form
    <num>  <time_unit>
    where <time_unit> is one of
    year(s) month(s) day(s) week(s) hour(s) minute(s) second(s)
    and <num> is an integer
    Example: 1 year  2 years 3 weeks -3 hours
*/

#define _GNU_SOURCE
#include "common_hdrs.h"

int parse_time_adjustment( char* datestring, struct tm* datetm )
{
    char* delim   = " \t";     /* Space and tab             */
    char* token;               /* Returned token            */
    int   number;              /* To store number token     */
    char  err_msg[STRING_MAX]; /* For error messages        */
    int   flags   = ONLY_DIGITS | NO_TRAILING;
    int   res;                 /* Return value of get_int() */
    /* Initialize strtok() and set token to first word in string.           */
    token = strtok(datestring, delim);

    /* Processing ends if strtok() returns a NULL pointer. */
    while ( token != NULL ) {
        /* Expecting an integer */
        res =  get_int(token, flags, &number, err_msg );
        if ( VALID_NUMBER != res )
            fatal_error(res, err_msg);

        /* num is quantity of time-adjustment unit to be found next
           get next token in time adjustment, should be a time unit.        */
        token = strtok(NULL, delim);
        if ( token == NULL ) {
             /* end of string encountered without the time unit*/
            fatal_error(TIME_ADJUST_ERROR, "missing a time unit\n");
        }

        /* Add num units to member of struct tm. */
        if ( NULL != strstr(token, "year"))
            datetm->tm_year += number;
        else if ( NULL != strstr(token, "month"))
            datetm->tm_mon += number;
        else if ( NULL != strstr(token, "week"))
            datetm->tm_mday += 7*number;
        else if ( NULL != strstr(token, "day"))
            datetm->tm_mday += number;
        else if ( NULL != strstr(token, "hour"))
            datetm->tm_hour += number;
        else if ( NULL != strstr(token, "minute"))
            datetm->tm_min += number;
        else if ( NULL != strstr(token, "second"))
            datetm->tm_sec += number;
        else
            /* Time_unit did not match any valid time time_unit.            */
            fatal_error(TIME_ADJUST_ERROR,
              "Found invalid time time_unit in amount to adjust the time\n");
        token = strtok(NULL, delim);
    }
    return 0;
}

int update_time( struct tm* datetm, struct tm* date_to_add )
{
    datetm->tm_year += date_to_add->tm_year;
    datetm->tm_mon  += date_to_add->tm_mon;
    datetm->tm_mday += date_to_add->tm_mday;
    datetm->tm_hour += date_to_add->tm_hour;
    datetm->tm_min  += date_to_add->tm_min;
    datetm->tm_sec  += date_to_add->tm_sec;

    errno = 0;
    mktime(datetm);
    if ( errno != 0 )
        fatal_error(errno, NULL);

    return 0;
}


int  main(int argc, char *argv[])
{
    char       formatted_date[MAXLEN];/* String storing formatted date      */
    time_t     current_time;          /* Timeval in seconds since Epoch     */
    struct tm *bdtime;                /* Broken-down time                   */
    struct tm  time_adjustment= {0};  /* Broken-down time for adjustment    */
    char       format_string[MAXLEN]; /* String for optional format spec    */
    char       usage_msg[512];        /* Usage message                      */
    char       ch;                    /* For option handling                */
    char       options[] = ":d:h";    /* getopt string                      */
    BOOL       d_option = FALSE;      /* Flag to indicate -d found          */
    char       *d_arg;                /* Dynamic string for -d argument     */
    int        d_arg_length;          /* Length of -d argument string       */

    opterr = 0;  /* turn off error messages by getopt() */

    while  (TRUE) {
        /* Call getopt, passing argc and argv and the options string.       */
        ch = getopt(argc, argv, options);
        if ( -1 == ch ) /* It returns -1 when it finds no more options.     */
            break;

        switch ( ch ) {
        case 'd':   /* Has required argument. */
            d_option = TRUE;
            d_arg_length = strlen(optarg);
            d_arg = malloc(d_arg_length * sizeof(char));
            if ( NULL == d_arg )
                fatal_error(EXIT_FAILURE,
                     "calloc could not allocate memory\n");
            strcpy(d_arg, optarg);
            break;
        case 'h':   /* Help message */
            sprintf(usage_msg, "%s [-d <time adjustment>]"
                   " [+\"format specification\"]", basename(argv[0]));
            usage_error(usage_msg);
        case '?' :
            fprintf(stderr,"Found invalid option %c\n", optopt);
            sprintf(usage_msg, "%s [-d <time adjustment>]"
                   " [+\"format specification\"]", basename(argv[0]));
            usage_error(usage_msg);
            break;
        case ':' :
            fprintf(stderr,"Missing required argument to -d\n");
            sprintf(usage_msg, "%s [-d <time adjustment>]"
                   " [+\"format specification\"]", basename(argv[0]));
            usage_error(usage_msg);
            break;
        }
    }

    if (optind < argc) {
        /* Non-option argument found - should be format specification.*/
       if ( argv[optind][0] == '+' ) /* argument starts with + */
            strncpy(format_string, argv[optind]+1, MAXLEN-1);
        else {
            sprintf(usage_msg, "%s [-d <time adjustment>]"
                   " [+\"format specification\"]\n", basename(argv[0]));
            usage_error(usage_msg);
        }
    }
    else  /* No argument - use default */
        strcpy(format_string,  FORMAT);

    /* Get the current time.           */
    current_time = time(NULL);

    /* Convert the current time into broken-down time. */
    bdtime = localtime(&current_time);

    /* The only possible error is EOVERFLOW, and localtime returns NULL
       if the error occurred. */
    if (bdtime == NULL)
        fatal_error(EOVERFLOW, "localtime");


    if ( d_option ) {
        parse_time_adjustment(d_arg,  &time_adjustment );
        update_time(bdtime, &time_adjustment);
        free(d_arg); /* Allocated in option handling above. */
    }

    /* Create a string from the broken down time using the %c format.       */
    if (0 == strftime(formatted_date, sizeof(formatted_date),
             format_string, bdtime) ) {
        /* strftime does not set errno. If return is 0, it is an error
           because we expect a non-zero number of bytes in the output
           string. */
        fatal_error(BAD_FORMAT_ERROR,
                     "Conversion to a date-time string failed or produced "
                        " an empty string\n");
    }

    /* Print the formatted date to standard output.  */
    printf("%s\n", formatted_date);
    return 0;
}
