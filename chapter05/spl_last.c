/******************************************************************************
  Title          : spl_last.c
  Author         : Stewart Weiss
  Created on     : June 29, 2023
  Description    : Implements the last command
  Purpose        : To with system data files
  Usage          : spl_last
  Build with     : gcc -I../include -L../lib -o spl_last   spl_last.c -lspl


******************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
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
#include <paths.h>
#include <utmpx.h>

/* Some systems define a record type of SHUTDOWN_TIME. If it's not defined
   define it.                                                              */
#ifndef SHUTDOWN_TIME
    #define SHUTDOWN_TIME 32 /* Give it a value larger than the other types */
#endif

/* Type definition for the linked list of utmpx records. */
struct utmp_list{
    struct utmpx ut;
    struct utmp_list *next;
    struct utmp_list *prev;
};

typedef struct utmp_list utlist;

/*  For debugging only, not used in finished program:
    print_rec_type prints the string representation of the integer value
    of utmp type  */
void print_rec_type( int t)
{
    switch (t)
    {
    case RUN_LVL:      	printf("RUN_LVL       "); break;
    case BOOT_TIME:     printf("BOOT_TIME     "); break;
    case NEW_TIME:      printf("NEW_TIME      "); break;
    case OLD_TIME:      printf("OLD_TIME      "); break;
    case INIT_PROCESS:  printf("INIT_PROCESS  "); break;
    case LOGIN_PROCESS: printf("LOGIN_PROCESS "); break;
    case USER_PROCESS:  printf("USER_PROCESS  "); break;
    case DEAD_PROCESS:  printf("DEAD_PROCESS  "); break;
    case ACCOUNTING:    printf("ACCOUNTING    "); break;
    }
}

/** get_prev_utrec(fd, ut, done)
    get_prev_utrec(fd, ut) reads one utmpx structure from the current file
    offset  into the address ut and repositions the file offset so that
    the next time it is called it reads the utmpx structure preceding the
    one it just read.
    Returns 1 on successful read and 0 if it could not read.
    If it returns 0, it sets *done to true when it has read the first record
    in the file.
*/
int get_prev_utrec(int fd, struct utmpx *ut, BOOL *finished )
{
    static off_t  saved_offset;    /* Where this call is about to read    */
    static BOOL   is_first = TRUE; /* Whether this is first time called   */
    size_t utsize = sizeof(struct utmpx); /* Size of utmpx struct */
    ssize_t       nbytes_read;     /* Number of bytes read                */

    /* Check if this is the first time it is called.
       If so, move the file offset to the last record in the file
       and save it in saved_offset. */
    if ( is_first ) {
        errno = 0;
        /* Move to utsize bytes before end of file. */
        saved_offset = lseek(fd, -utsize, SEEK_END);
        if ( -1 == saved_offset )  {
            error_mssge(1, "error trying to move offset to last rec of file");
            return FALSE;
        }
        is_first = FALSE; /* Turn off flag. */
    }

    *finished = FALSE;       /* Assume we're not done yet. */
    if ( saved_offset < 0 ) {
        *finished = TRUE;   /* saved_offset < 0 implies we've read entire file. */
        return FALSE;    /* Return 0 to indicate no read took place.         */
    }
    /* File offset is at the correct place to read. */
    errno = 0;
    nbytes_read = read(fd, ut, utsize);
    if ( -1 == nbytes_read ) {
        /* read() error occurred; do not exit - let main() do that. */
        error_mssge(errno, "read");
        return FALSE;
    }
    else if ( nbytes_read < utsize ) {
        /* Full utmpx struct not read; do not exit - let main() do that. */
        error_mssge(READ_ERROR, "less than full record read");
        return FALSE;
    }
    else { /* Successful read of utmpx record */
        saved_offset = saved_offset - utsize; /* Reposition saved_offset. */
        if ( saved_offset >= 0 ) {
            /* Seek to preceding record to set up next read. */
            errno = 0;
            if ( -1 == lseek(fd, - (2*utsize), SEEK_CUR) )
                fatal_error(errno, "lseek()");
        }
        return TRUE;
    }
}

/** format_time_diff(start, finish, str)
    creates a string representing the number of seconds from start to
    finish.
    The string is of the form "(days+HH:MM)" where "days+" is omitted if
    days == 0, and is assigned to the str parameter.
*/
void format_time_diff( time_t start_time, time_t end_time, char* time_diff_str)
{
    time_t secs;
    int    minutes;
    int    hours;
    int    days;

    secs     = end_time - start_time;
    minutes  = (secs / 60) % 60;
    hours    = (secs / 3600) % 24;
    days     = secs / 86400;

    /* If days > 0 then use a different format. */
    if ( days > 0 )
        sprintf(time_diff_str, "(%d+%02d:%02d)", days, hours, minutes);
    else
        sprintf(time_diff_str, "(%02d:%02d)", hours, minutes);
}

/**
    print_one_line(u,t) prints one line of output representing
    the utmpx record u for a session that starts at time u.ut_tv.tv_sec
    and ends at time t, both time_t values.
*/
void print_one_line(struct utmpx *ut, time_t end_time)
{
    time_t     utrec_time;
    struct tm *bd_end_time;
    struct tm *bd_ut_time;
    char       formatted_login[MAXLEN];     /* String storing formatted login date   */
    char       formatted_logout[MAXLEN];    /* String storing formatted logout date  */
    char       duration[MAXLEN];            /* String representig session length     */
    char       *start_date_fmt = "%a %b %d %H:%M";
    char       *end_date_fmt   = "%H:%M";

    utrec_time = (ut->ut_tv).tv_sec;        /* Get login time, in seconds */

    /* If the end time is 0 or -1, print the appropriate string instead of a time */
    if ( ut->ut_type == BOOT_TIME && end_time == 0 )
        sprintf(duration, "still running");
    else if ( ut->ut_type == USER_PROCESS && end_time == 0 )
        sprintf(duration, "still logged in");
    else if ( ut->ut_type == USER_PROCESS && end_time == -1 )
        sprintf(duration, "gone - no logout");
    else /* Calculate and format duration of the session. */
        format_time_diff(utrec_time, end_time, duration);

    bd_ut_time     = localtime(&utrec_time); /* Convert login time to broken-down time. */
    if (bd_ut_time == NULL)
        fatal_error(errno, "localtime");

    if (0 == strftime(formatted_login, sizeof(formatted_login),
             start_date_fmt, bd_ut_time) )
        fatal_error(BAD_FORMAT_ERROR,
                     "Conversion to a date-time string failed or produced "
                        " an empty string\n");
    bd_end_time = localtime(&end_time);   /* Convert end time to broken-down time. */
    if (bd_end_time == NULL)
        fatal_error(errno, "localtime");

    if (0 == strftime(formatted_logout, sizeof(formatted_logout),
             end_date_fmt, bd_end_time) )
        fatal_error(BAD_FORMAT_ERROR,
                     "Conversion to a date-time string failed or produced "
                        " an empty string\n");

    ut->ut_host[sizeof(ut->ut_host)-1] = '\0';
    printf("%-8.8s %-12.12s %-18s %s - %s %s\n", ut->ut_user, ut->ut_line,
            ut->ut_host, formatted_login, formatted_logout, duration);
}

void save_ut_to_list(struct utmpx *ut,  utlist **list)
{
    utlist* utmp_node_ptr;

    /* Allocate a new list node. */
    errno = 0;
    if ( NULL == (utmp_node_ptr = (utlist*) malloc(sizeof(utlist)) ) )
        fatal_error(errno, "malloc");

    /* Copy the utmpx record into the new node. */
    memcpy(&(utmp_node_ptr->ut), ut, sizeof(struct utmpx));

    /* Attached the node to the front of the list. */
    utmp_node_ptr->next  = *list;
    utmp_node_ptr->prev  = NULL;
    if (NULL != *list)
        (*list)->prev = utmp_node_ptr;
    (*list) = utmp_node_ptr;
}

void delete_utnode(utlist* p, utlist** list)
{
    if ( NULL != p->next )
        p->next->prev = p->prev;

    if ( NULL != p->prev )
        p->prev->next = p->next;
    else
        *list = p->next;
    free(p);
}

void erase_utlist(utlist **list)
{
    utlist *ptr = *list;
    utlist *next;

    while ( NULL != ptr ) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
    *list = NULL;
}


int main( int argc, char *argv[] )
{

    struct utmpx  utmp_entry;              /* Read info into here             */
    size_t        utsize = sizeof(struct utmpx); /* Size of utmpx record      */
    int           fd_utmp;                 /* Read from this descriptor       */
    time_t        last_boot_time;          /* Time of last boot or reboot     */
    time_t        last_shutdown_time = 0;  /* Time of last shutdown           */
    time_t        start_time;              /* When wtmp processing started    */
    struct tm    *bd_start_time;           /* Broken-down time representation */
    char          wtmp_start_str[MAXLEN];       /* String to store start time */
    utlist        *saved_ut_recs = NULL;   /* An initially empty list         */
    char          options[] = ":x";        /* getopt string                   */
    int           show_sys_events = FALSE; /* Flag to indicate -x found       */
    char          usage_msg[MAXLEN];       /* For error messages              */
    BOOL          done = FALSE;            /* Flag to stop utmp loop          */
    BOOL          found = FALSE;           /* Flag to indicate match found    */
    char          ch;
    utlist        *p, *next;


    if ( (fd_utmp = open(WTMPX_FILE, O_RDONLY)) == -1 ) {
        fatal_error(errno, "while opening "  WTMPX_FILE);
    }

    /* Check options */
    opterr = 0;  /* Turn off error messages by getopt() */

    while  (TRUE) {
        /* Call getopt, passing argc and argv and the options string. */
        ch = getopt(argc, argv, options);
        if ( -1 == ch ) /* It returns -1 when it finds no more options.  */
            break;

        switch ( ch ) {
        case 'x':
            show_sys_events = TRUE;
            break;

        case '?' :
        case ':' :
            fprintf(stderr,"Found invalid option %c\n", optopt);
            sprintf(usage_msg, "%s [ -x ]", basename(argv[0]));
            usage_error(usage_msg);
            break;
        }
    }

    /* Set the locale. */
    char*  mylocale;
    if ( (mylocale = setlocale(LC_TIME, "") ) == NULL )
        fatal_error( LOCALE_ERROR, "setlocale() could not set the given locale");

    /* Read the first structure in the file to capture the time of the
       first entry. */
    errno = 0;
    if ( read(fd_utmp, &utmp_entry, utsize) != utsize )
        fatal_error(errno, "read");

    start_time = utmp_entry.ut_tv.tv_sec ;

    /* Process the wtmp file */
    while ( !done ) {
        errno = 0;
        if ( get_prev_utrec(fd_utmp, &utmp_entry, &done)  ) {
            /*
               What type of record is this?
               For ordinary user logins, the ut_type field will be USER_PROCESS
               but for shutdown events, there is no SHUTDOWN_TIME.
               We can identify a shutdown record by the line being ~ and
               the username being "shutdown".
             */

            if ( (strncmp(utmp_entry.ut_line, "~", 1) == 0) &&
                 (strncmp(utmp_entry.ut_user, "shutdown", 8) == 0) ) {
                utmp_entry.ut_type = SHUTDOWN_TIME;
                sprintf(utmp_entry.ut_line, "system down");
            }

            switch (utmp_entry.ut_type) {
            case BOOT_TIME:
                strcpy(utmp_entry.ut_line, "system boot");
                print_one_line(&utmp_entry, last_shutdown_time);
                last_boot_time = utmp_entry.ut_tv.tv_sec;
                if ( saved_ut_recs != NULL )
                    erase_utlist(&saved_ut_recs);
                break;

            case RUN_LVL:
                break;
            case SHUTDOWN_TIME:
                last_shutdown_time = utmp_entry.ut_tv.tv_sec;
                if ( show_sys_events )
                    print_one_line(&utmp_entry, last_boot_time);
                if ( saved_ut_recs != NULL )
                    erase_utlist(&saved_ut_recs);
                break;
            case USER_PROCESS:
                /* Find the logout entry for this login in the saved_ut_recs
                   list. This should be the entry closest to the front of the
                   list with the same ut_line field. */
                found = FALSE;
                p = saved_ut_recs; /* start at beginning */
                while ( NULL != p ) {
                    next = p->next;
                    if ( 0 == (strncmp(p->ut.ut_line, utmp_entry.ut_line,
                        sizeof(utmp_entry.ut_line)) ) ) {
                        /* The saved node's ut_line matches the one we just found */
                        print_one_line(&utmp_entry, p->ut.ut_tv.tv_sec);
                        found = TRUE;
                        delete_utnode(p, &saved_ut_recs); /* Delete the node */
                    }
                    p = next;
                }
                if ( !found ) {
                    /* No logout record found for this login.
                       If the system was not shut down after this login,
                       there is no record because the user is still
                       logged in.
                       If the system was shut down at any time after this login,
                       the user cannot still be logged into the same session.
                       If there is no saved logout record, it implies that
                       the session ended in an abnormal way and the output
                       should just indicate the user is "gone".
                     */
                    if ( last_shutdown_time > 0 )
                        print_one_line(&utmp_entry, (time_t) -1);
                    else
                        print_one_line(&utmp_entry, (time_t) 0);
                }
                break;
            case DEAD_PROCESS:
                /* Create a node in the saved_ut_recs list for this entry,
                   provided that the ut_line field is not null. */
                if ( utmp_entry.ut_line[0] == 0 )
                    /* There is no line in the entry, so skip it. */
                    continue;
                else
                    save_ut_to_list(&utmp_entry, &saved_ut_recs);
                break;

            case OLD_TIME:
            case NEW_TIME:
            case INIT_PROCESS:
            case LOGIN_PROCESS:
                break;

            }
        }
        else /* get_prev_utrec() did not read. */
            if ( !done )
                fatal_error(2, " read failed");

    }
    erase_utlist(&saved_ut_recs);
    close(fd_utmp);

    bd_start_time = localtime(&start_time);
    if (0 == strftime(wtmp_start_str, sizeof(wtmp_start_str),
         "%a %b %d %H:%M:%S %Y", bd_start_time) )
    fatal_error(BAD_FORMAT_ERROR,
                 "Conversion to a date-time string failed or produced "
                    " an empty string\n");
    printf("\nwtmp begins %s\n", wtmp_start_str);
    return 0;
}
