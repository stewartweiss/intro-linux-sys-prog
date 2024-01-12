/*****************************************************************************
  Title          : spl_lastlog.c
  Author         : Stewart Weiss
  Created on     : June 19, 2023
  Description    : Displays the contents of the lastlog file
  Purpose        : To work with files containing holes
  Usage          : spl_lastlog
  Build with     : gcc -I../include -L../lib -o spl_lastlog   spl_lastlog.c \
                    -lspl

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

#define _GNU_SOURCE
#include "common_hdrs.h"
#include <lastlog.h>              /* For lastlog structure definition       */
#include <paths.h>                /* For definition of _PATH_LASTLOG        */
#include <pwd.h>                  /* For password file iterators            */

#define MESSAGE_SIZE   512

/* This function prints a line for a username who has never logged in,
   matching the format of the line to one in which there is a port and host
   to be printed, so that things line up. It approximates the format of
   the actual lastlog command. */
void print_never_logged_in(char* uname)
{
    printf("%-16s %-8.8s %-16s **Never logged in**\n", uname, " ", " ");
}

int main(int argc, char *argv[])
{
    struct lastlog ll_entry;      /* To store lastlog record read from file */
    struct passwd  *psswd_struct; /* passwd structure from password file    */
    int            ll_fd;         /* File descriptor of lastlog file        */
    off_t          ll_file_size;  /* Size of lastlog file, in bytes         */
    size_t         ll_struct_size;/* Size in bytes of lastlog structure     */
    size_t         num_bytes;     /* Number of bytes read in read()         */
    uid_t          uid;           /* Userid of current search               */
    char           *username;     /* Username of current search             */
    int            highest_uid;   /* Highest userid in lastlog file         */
    char           *mylocale;     /* Pointer to current locale              */
    char           lastlog_time[64];/* Localized date/time string           */
    time_t         ll_time;       /* Lastlog time converted to time_t       */
    struct tm      *bdtime;       /* broken-down time                       */

    /* Open the lastlog file and handle potential errors. */
    errno = 0;
    if ( (ll_fd = open(_PATH_LASTLOG, O_RDONLY)) == -1 ) {
        fatal_error(errno, "while opening "  _PATH_LASTLOG);
    }

    /* Get the size of the lastlog file. */
    ll_file_size = lseek(ll_fd,0, SEEK_END);

    /* Get size of lastlog struct. */
    ll_struct_size = sizeof(struct lastlog);

    /* Get highest userid currently stored in lastlog file. */
    highest_uid = ll_file_size/ll_struct_size - 1;

    if ( (mylocale = setlocale(LC_ALL, "") ) == NULL )
        fatal_error( LOCALE_ERROR,
                      "setlocale() could not set the given locale");

    /* Initialize the passwd file iterator. */
    setpwent();

    /* Print a header line, matching the spacing of the data lines. */
    printf("Username         Port     From             Last Login\n");

    while ( (psswd_struct  = getpwent()) != NULL) {
        uid = psswd_struct->pw_uid;
        username = psswd_struct->pw_name;

        if ( uid > highest_uid ) {
            print_never_logged_in(username);
        }
        else {
            if (lseek(ll_fd, uid * ll_struct_size, SEEK_SET) == -1)
                fatal_error(errno, "lseek");

            errno = 0;
            if ((num_bytes = read(ll_fd, &ll_entry, ll_struct_size)) <= 0) {
                if ( 0 != errno ) /* A read error occurred */
                    fatal_error(errno, "read");
                else { /* Not a read error - shouldn't happen but continue */
                    error_mssge(-1,"could not read the entry, skipping");
                    continue;
                }
            }
            else if ( num_bytes != ll_struct_size) {
                fatal_error(READ_ERROR, "Incomplete read of lastlog struct");
            }

            if (0 == ll_entry.ll_time) {
                /* No entry for this user */
                print_never_logged_in(username);
            }
            else {
                /* Convert the lastlog time into broken-down time. */
#if __WORDSIZE_TIME64_COMPAT32
                ll_time = ll_entry.ll_time;
                bdtime = localtime(&ll_time);
#else
                bdtime = localtime(&(ll_entry.ll_time));
#endif

                /* The only possible error is EOVERFLOW, and localtime
                   returns NULL if the error occurred. */
                if (bdtime == NULL)
                    fatal_error(EOVERFLOW, "localtime");

                /* Create a string from the broken down time using the
                   %c format */
                if (0 == strftime(lastlog_time, sizeof(lastlog_time),
                         FORMAT, bdtime) )
                    /* strftime does not set errno. If return is 0, it is an
                       error because we expect a non-zero number of bytes in
                       the output string. */
                    fatal_error(-1, "Conversion to a date-time string failed "
                              " or produced an empty string\n");

                printf("%-16s %-8.8s %-16s %s\n", username, ll_entry.ll_line,
                      ll_entry.ll_host, lastlog_time);
            }
        }
    }
    close(ll_fd);
    exit(EXIT_SUCCESS);
}
