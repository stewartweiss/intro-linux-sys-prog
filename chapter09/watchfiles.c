/*****************************************************************************
  Title          : watchfiles.c
  Author         : Stewart Weiss
  Created on     : February 16, 2024
  Description    : Displays files and their sizes, updating dynamically
  Purpose        : To show how to work with timers
  Usage          : watchfiles  file file ...
  Build with     : gcc -Wall -g -I ../include watchfiles.c -o watchfiles \
                   -L../lib -lspl -lrt

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



#define _GNU_SOURCE       /* Needed to expose statx() function in glibc */
#include <sys/stat.h>     /* Required according to man page for statx() */
#include <termios.h>      /* Needed for tcflush()                       */
#include "common_hdrs.h"
#include "time_utils.h"
#include "get_nums.h"

/* Cursor movements and screen clearing macros using ANSI escape sequences */
#define    REFRESH          write(STDOUT_FILENO, "\033[1J", 4)
#define    CLEARDISPLAY     write(STDOUT_FILENO, "\033[2J", 4)
#define    MOVETOHOME       write(STDOUT_FILENO, "\033[1;1H", 6)
#define    MOVETO(seq)      write(STDOUT_FILENO, "\033[",2); \
                            write(STDOUT_FILENO, seq, strlen(seq));\
                            write(STDOUT_FILENO, ";1H",3)


char     **filelist;          /* Files for processing                       */
char     **displayname;       /* Name to display in case path too long      */
int        numfiles = 0;      /* Number of files to be processed            */
char       linecnt_str[3];    /* String storing count of lines to refresh   */
long long *prevsize;          /* Array of file sizes in previous update     */
int        stable_cnt = 0;    /* Count of consecutive unchanged updates     */
int        nochange_limit;    /* When stable_cnt reaches it, time to stop   */
const int  MAX_LENGTH = 50;   /* Width of filename field in output          */
char       heading[66];       /* Heading with column labels to be displayed */
char       rownum[3];         /* Row number of line containing the prompt   */
volatile sig_atomic_t  stopflag = 0; /* Flag that handler sets to stop main */


void cleanup()
{
    int i;
    for ( i = 0; i < numfiles; i++)
        free(displayname[i]);
    free(displayname);
    free(filelist);
    free(prevsize);
}

/* Signal handler for SIGUSR1 -- updates the screen with new sizes.         */
void update_stats( int signum )
{
    const char prompt[]     = "Enter \'q\' to quit:";
    struct stat statbuf;    /* stat structure filled by statx()        */
    char   outbuffer[4096]; /* String to display with every update     */
    char   one_line[256];   /* Buffer to store one file's line         */
    long long newfilesize;  /* New size of file                        */
    char   size_str[20];    /* Size of filename stored as a string     */
    static BOOL changed;    /* Flag to indicate if a file has changed  */
    int    i = 0;

    MOVETO(rownum);         /* Move cursor to prompt line.             */
    REFRESH;                /* Clear the screen from top to this line. */
    MOVETOHOME;             /* Move cursor to upper-left.              */
    changed = FALSE;
    strcpy(outbuffer, heading); /* Copy heading to buffer. */
    for ( i = 0; i < numfiles; i++) {
        if ( lstat(filelist[i], &statbuf) < 0 )
            if ( errno == ENOENT ) {
                strcpy(one_line, displayname[i]);
                strcat(one_line,"   disappeared\n");
            }
            else {
                strcpy(one_line, displayname[i]);
                strcat(one_line,"   stat failed\n");
            }
        else {  /* Valid stat of file */
            newfilesize = statbuf.st_size;       /* Get new size.      */
            if ( prevsize[i] != newfilesize ) {  /* Size changed.      */
                changed = TRUE;                  /* Make a note of it. */
                prevsize[i] = newfilesize ;      /* Save new size.     */
            }
            lltostr(newfilesize, size_str, 15);
            strcpy(one_line, displayname[i]);
            strcat(one_line, size_str);
            strcat(one_line, "\n");
        }
        strncat(outbuffer, one_line, strlen(outbuffer));
    }
    /* Write the outbuffer to the terminal display. */
    if ( -1 == write(STDOUT_FILENO, outbuffer, strlen(outbuffer)) )
        fatal_error(errno, "write");

    if ( ! changed )
        stable_cnt++;    /* No file changed in this update increment count. */
    else
        stable_cnt = 0;   /* A file changed - reset count to start again.   */
    if ( stable_cnt >= nochange_limit )
        stopflag = 1;
    MOVETO(rownum);       /* Move cursor to line  where prompt is written.  */
    write(STDOUT_FILENO, prompt,  strlen(prompt));       /* Display prompt. */
}

void sig_handler( int signum )
{
    cleanup();
    raise(SIGTERM);
}



/*--------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
    char   usage_mssge[128];    /* String to store usage message            */
    struct sigaction act;       /* For setting up signal handler            */
    struct sigevent  sev;       /* For creating the timer                   */
    timer_t          timerid;   /* For creating the timer                   */
    struct timespec  refresh_ts;
    struct timespec  ZERO_TS = {0,0};    /* A timespec of zero length       */
    struct itimerspec refresh_interval;  /* Timer val, defaults to 1 second */
    int i,n, k = 0;
    struct stat statbuf;        /* Buffer for lstat()                       */
    char   c;
    double refresh_secs = 1.0;           /* Default refresh interval        */
    char  options[] = ":l:i:";
    char  stopstr[32];
    char  intervalstr[32];
    char  errmssge[128];
    size_t len;


    sprintf(usage_mssge, "%s [ -i interval | -l limit ] file [file...]",
              basename(argv[0]));
    /* Create the heading. */
    strcpy(heading,
        "File                                                        Size\n");
    nochange_limit = 5;
    opterr = 0;
    while  (TRUE) {
        c = getopt(argc, argv, options);
        if ( -1 == c ) /* It returns -1 when it finds no more options.  */
            break;

        switch ( c ) {
        case 'l':
            strncpy(stopstr, optarg, strlen(optarg));;
            if ( get_int(stopstr, NON_NEG_ONLY, &nochange_limit,
                 errmssge ) < 0 )
                fatal_error(-1, errmssge);
            break;
        case 'i':
            strncpy(intervalstr, optarg, strlen(optarg));
            if ( get_dbl(intervalstr, NON_NEG_ONLY, &refresh_secs,
                errmssge ) < 0 )
                fatal_error(-1, errmssge);
            break;
        case '?' :
        case ':' :
            fprintf(stderr,"Found invalid option %c\n", optopt);
            usage_error(usage_mssge);
            break;
        }
    }

    /* If no file arguments, print a usage message and exit. */
    if (argc <= optind ) {
        usage_error(usage_mssge);
    }

    /* Initialize filelist and its size. */
    numfiles = argc - optind;
    if ( (filelist = calloc(numfiles, sizeof(char*)) ) == NULL )
        fatal_error(errno, "calloc");

    /* Check that each file in the argument list can be stat-ed before we
       start monitoring. */
    for ( k = 0, i = optind; i < argc; i++ ) {
        if ( lstat(argv[i], &statbuf) < 0 ) {
            fprintf(stderr, "Could not stat %s, skipping it. \n", argv[i]);
            numfiles--;
        }
        else {
            filelist[k++] = argv[i];
        }
    }

    /* If no stat-able files then exit. */
    if ( 0 == numfiles )
        fatal_error(-1, "No files to watch, exiting.\n");

    sprintf(rownum, "%d", numfiles+2);

    if ( (displayname = calloc(numfiles, sizeof(char*)) ) == NULL )
        fatal_error(errno, "calloc");
    if ( (prevsize = calloc(numfiles, sizeof(long long int)) ) == NULL )
        fatal_error(errno, "calloc");

    for ( i =0; i < numfiles; i++ ) {
        len = strlen(filelist[i]);
        displayname[i] = calloc(MAX_LENGTH+1, sizeof(char));
        if ( len > MAX_LENGTH ) {
            sprintf(displayname[i], "...%*s",
               MAX_LENGTH-3, filelist[i]+len-MAX_LENGTH+3);
        }
        else {
            memset(displayname[i], ' ', MAX_LENGTH);
            displayname[MAX_LENGTH] = '\0';
            strncpy(displayname[i], filelist[i], len);;
        }
        /* The length of displayname[i] should be MAX_LENGTH always now */
    }

    /* Set up signal handling. */
    act.sa_handler = sig_handler;
    act.sa_flags   = 0;
    sigemptyset(&(act.sa_mask));
    if ( sigaction(SIGINT, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");
    if ( sigaction(SIGQUIT, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    CLEARDISPLAY;  /* Clear entire screen. */

    /* Install update_stats as the SIGUSR1 handler */
    act.sa_handler = update_stats;
    if ( sigaction(SIGUSR1, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo  = SIGUSR1;
    sev.sigev_value  = (union sigval) 0;

    /* Create a timer with this sigevent structure.  */
    if (-1 == timer_create(CLOCK_MONOTONIC, &sev, &timerid) )
       fatal_error(errno, "timer_create");

    /* Convert refresh_secs length to a timespec. */
    dbl_to_timespec(refresh_secs, &refresh_ts);


    /* Set up the timer's initial value and repeat interval to be the same. */
    refresh_interval.it_value    = refresh_ts;
    refresh_interval.it_interval = refresh_ts;

    /* Arm the timer just created with the itimerspec interval. */
    if (-1 == timer_settime(timerid, 0, &refresh_interval, NULL) )
        fatal_error(errno, "timer_settime");

    while ( !stopflag ) {
        n = read(STDIN_FILENO, &c, 1);
        if ( -1 == n && errno == EINTR )
            continue;
        else if ( c == 'q' )
            break;

    }
    tcflush(STDIN_FILENO, TCIFLUSH); /* Remove last newline character. */
    refresh_interval.it_value = ZERO_TS;
    if (-1 == timer_settime(timerid, 0, &refresh_interval, NULL) )
        fatal_error(errno, "timer_settime");

    if ( stopflag )
        printf("\nNo changes were detected in the last %2.1f"
               " seconds in any file.\n", nochange_limit*refresh_secs);
    cleanup();
    exit(EXIT_SUCCESS);
}
