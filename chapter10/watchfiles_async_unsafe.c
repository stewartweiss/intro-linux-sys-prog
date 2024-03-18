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

/* ANSI Escape Sequence for clearing the screen and its length  */
const char CLEAR_SCREEN[] = "\033[2J";
const int  lCLEAR_SCREEN  =  4;
const char CLEAR_TO_CURSOR[] = "\033[1J";
#define    REFRESH  write(STDOUT_FILENO,CLEAR_TO_CURSOR, 4)
#define    CLEARDISPLAY  write(STDOUT_FILENO,CLEAR_SCREEN, lCLEAR_SCREEN)

char     **filelist;          /* Files for processing                       */
char     **shortnam;          /* Name for file to display if too long       */
int        numfiles = 0;      /* Number of files to be processed            */
int        maxpathlen = 0;    /* Length of longest pathname                 */
long long *prevsize;          /* Array of file sizes in previous update     */
int        nochange_count = 0; /* Count of consecutive updates with no change */
int        nochange_limit;
volatile sig_atomic_t  stopflag = 0;
const int  MAX_LENGTH = 50;

/* Move the cursor to the screen position (line,col) (upper left is (0,0)   */
void moveto(int line, int col )
{
    char seq_str[20];

    sprintf(seq_str, "\033[%d;%dH", line, col);
    write(STDOUT_FILENO, seq_str, strlen(seq_str));
}

/* Signal handler for SIGUSR1 -- updates the screen with new sizes.         */
void update_stats( int signum )
{
    const char prompt[] = "Enter \'q\' to quit:";
    struct stat statbuf;    /* statx structure filled by statx()      */
    char   update_buffer[4096];
    char   one_line[80];
    int    i;
    long long newfs;
    static BOOL changed;   /* Flag to indicate if a file has changed*/
    char   size_str[20];

    moveto(numfiles+2,1);
    REFRESH;
    moveto(1,1);
    changed = FALSE;
    sprintf(update_buffer, "File%*s\n", MAX_LENGTH+10, "Size");
    for ( i = 0; i < numfiles; i++) {
        if ( lstat(filelist[i], &statbuf) < 0 )
            if ( errno == ENOENT )
                sprintf(one_line, "%-*s%14s\n", MAX_LENGTH,shortnam[i],
                        "disappeared");
            else
                sprintf(one_line, "%-*s%14s\n", MAX_LENGTH,shortnam[i],
                        "stat failed");
        else {  /* Valid stat of file */
            newfs = statbuf.st_size;     /* Get new size. */
            if ( prevsize[i] != newfs ) {  /* Size changed. */
                changed = TRUE;
                prevsize[i] = newfs ;
            }
            sprintf(size_str, "%14llu", newfs);
            sprintf(one_line, "%-*s%s\n", MAX_LENGTH, shortnam[i], size_str);
        }
        strncat(update_buffer, one_line, strlen(one_line));
    }
    if ( -1 == write(STDOUT_FILENO, update_buffer, strlen(update_buffer)) )
        fatal_error(errno, "write");

    if ( ! changed )
        nochange_count++;  /* No file changed in this update increment count. */
    else
        nochange_count = 0; /* A file changed - reset count to start again.   */

    if ( nochange_count >= nochange_limit )
        stopflag = 1;
    moveto(numfiles+3,1);
    write(STDOUT_FILENO, prompt,  strlen(prompt));
}

void sig_handler( int signum )
{
    raise(SIGTERM);
}



/*--------------------------------------------------------------------------*/
int main(int argc, char **argv)
{

    char   usage_mssge[128];  /* string to store usage message            */
    struct sigaction act;
    struct sigevent  sev;
    timer_t      timerid;
    struct timespec  refresh_ts;
    struct timespec  ZERO_TS = {0,0};    /* A timespec of zero length */
    struct itimerspec refresh_interval;  /* Timer val = one second repeats */
    int i,k;
    struct stat statbuf;           /* buffer for statx()   */
    char   c;
    double refresh_secs = 1.0;           /* Default refresh interval        */
    int n;
    char  options[] = ":d:n:";
    char  stopstr[32];
    char  intervalstr[32];
    char  errmssge[128];

    sprintf(usage_mssge, "%s [ -n interval | -d count ] file [file...]",
              basename(argv[0]));
    nochange_limit = 5;
    opterr = 0;
    while  (TRUE) {
        c = getopt(argc, argv, options);
        if ( -1 == c ) /* It returns -1 when it finds no more options.  */
            break;

        switch ( c ) {
        case 'd':
            strncpy(stopstr, optarg, strlen(optarg));;
            if ( get_int(stopstr, NON_NEG_ONLY, &nochange_limit,
                 errmssge ) < 0 )
                fatal_error(-1, errmssge);
            break;
        case 'n':
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

    /* Initialize filelist and its size. filelist points to first argument. */
    filelist = argv + optind;
    numfiles = argc - optind;
    k = 0;

    /* Check that each file in the argument list can be stat-ed before we
       start monitoring. */
    for ( i = optind; i < argc; i++ ) {
        if ( lstat(argv[i], &statbuf) < 0 ) {
            fprintf(stderr, "Could not stat %s, skipping it. \n", argv[i]);
            numfiles--;
        }
        else {
            filelist[k] = argv[i];
            k++;
        }
    }

    /* If no stat-able files then exit. */
    if ( 0 == numfiles )
        fatal_error(-1, "No files to watch, exiting.\n");

    shortnam = calloc(numfiles, sizeof(char*));
    if ( NULL == shortnam )
        fatal_error(errno, "calloc");

    prevsize = calloc(numfiles, sizeof(long long int));
    if ( NULL == prevsize )
        fatal_error(errno, "calloc");

    /* Compute length of longest pathname. */
    for ( i =0; i < numfiles; i++ ) {
        size_t len = strlen(filelist[i]);
        if ( len > MAX_LENGTH ) {
            shortnam[i] = calloc(MAX_LENGTH, sizeof(char));
            sprintf(shortnam[i], "...%*s",
               MAX_LENGTH-3, filelist[i]+len-MAX_LENGTH+3);
            maxpathlen = MAX_LENGTH;
        }
        else {
            shortnam[i] = calloc(strlen(filelist[i])+1, sizeof(char));
            strncpy(shortnam[i], filelist[i], strlen(filelist[i]));
        }
        len = strlen(shortnam[i]);
        maxpathlen = len > maxpathlen ? len : maxpathlen;
    }

    /* Set up signal handling. */
    act.sa_handler = sig_handler;
    act.sa_flags   = 0; //SA_RESTART;
    if ( sigaction(SIGINT, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");
    if ( sigaction(SIGQUIT, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    CLEARDISPLAY;

    /* Install update_progressbar as the SIGUSR1 handler */
    act.sa_handler = update_stats;
    if ( sigaction(SIGUSR1, &act, NULL) == -1 )
        fatal_error(errno, "sigaction");

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo  = SIGUSR1;

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

    while ( TRUE ) {
        if ( stopflag )
           break;
        n = read(STDIN_FILENO, &c, 1);
        if ( -1 == n && errno == EINTR )
            continue;
        else if ( c == 'q' )
            break;

    }
    tcflush(STDIN_FILENO, TCIFLUSH);
    refresh_interval.it_value = ZERO_TS;
    if (-1 == timer_settime(timerid, 0, &refresh_interval, NULL) )
        fatal_error(errno, "timer_settime");

    if ( stopflag )
        printf("\nNo changes were detected in the last %2.1f seconds in any file.\n",
            nochange_limit*refresh_secs);

    return 0;
}
