/*****************************************************************************
  Title          : sigio_demo.c
  Author         : Stewart Weiss
  Created on     : August 3,2024
  Description    : Displays a moving char on  screen and accepts user input
  Purpose        : To show how to use signal-driven I/O
  Usage          : sigio_demo
  Build with     : gcc -Wall -L../lib -I../include -o sigio_demo \
                     sigio_demo.c -lspl

  Notes:
  This program will not behave "perfectly". If you type fast enough, you might
  see your typing in a row below the first row.

  The program should not be run in windows that are too small, and it prevents
  you from doing this. All bets are off if you resize the window while it's
  running.

******************************************************************************
* Copyright (C) 2024 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include "common_hdrs.h"
#include <fcntl.h>
#include <termios.h>
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

#define  FREQ_NS  100000000


volatile sig_atomic_t input_ready = 0;
volatile sig_atomic_t timer_expired = 0;

void moveto(int line, int col )
{
    char seq_str[20];
    sprintf(seq_str, "\033[%d;%dH", line, col);
    write(STDOUT_FILENO, seq_str, strlen(seq_str));
}

void setup_fd(int fd )
{
    int flagset   = fcntl(fd, F_GETFL);
    if ( flagset == -1 )
        fatal_error(errno, "fcntl");
    if ( -1 == fcntl(fd, F_SETFL, flagset | O_ASYNC | O_NONBLOCK))
        fatal_error(errno, "fcntl");
    fcntl(fd, F_SETOWN, getpid());
}

void on_input(int signum)
{
    input_ready = 1;
}

void on_timer(int signum)
{
    timer_expired = 1;
}


int  get_window_size( int ttyfd, int *rows, int *cols )
{
    struct winsize size;
    int retval;

    if ( -1 != ( retval = ioctl(ttyfd, TIOCGWINSZ,  &size) ) ) {
        *rows = size.ws_row;
        *cols = size.ws_col;
    }
    return retval;
}


int main( int argc, char * argv[])
{

    const char CLEAR_SCREEN[] = "\033[2J"; /* Escape seq to clear screen    */
    const char CLEAR_ABOVE[] =  "\033[1J"; /* Clears all lines above        */
    const int  TOP_ROW = 3;   /* Highest row in which sprite is displayed   */
    int   clr_above_len   = 4;
    struct sigaction sigact;             /* For installing handlers         */
    struct timespec refresh_timespec = {0, FREQ_NS};  /* Refresh rate       */
    struct itimerspec refresh_interval;  /* The timer value and repeat      */
    struct sigevent sev;                 /* Notification structure          */
    timer_t timerid;                     /* Timer ID from timer_create()    */
    char   ch;                           /* User input                      */
    BOOL   finished = FALSE;             /* Loop exit condition             */
    int  row = TOP_ROW, oldrow;          /* Drawing position row coordinate */
    int  col = 1, oldcol;                /* Drawing position col coordinate */
    char sprite = 'O';                   /* The sprite to draw              */
    char blank = ' ';                    /* For erasing                     */
    int  numrows;                        /* Window row dimension            */
    int  numcols;                        /* Window column dimension         */
    char msg[32];                        /* To print in bottom row          */
    int  user_row_adjust = 0;            /* Net row change caused by user   */

    if ( -1 == get_window_size(0, &numrows, &numcols))
        fatal_error(errno, "ioctl");
    else if ( numrows < 10 || numcols < 40 )
        usage_error("The window should be at least 10 rows by 40 columns.\n");

    /* Set up signal handling  */
    sigact.sa_flags = SA_RESTART;
    sigact.sa_handler = on_input;
    sigemptyset(&sigact.sa_mask);
    if ( sigaction(SIGIO, &sigact, NULL) == -1 )
        fatal_error(errno, "sigaction");

    sigact.sa_handler = on_timer; /* SIGALRM handler function */
    if ( sigaction(SIGUSR1, &sigact, NULL) == -1 )  /* try to install */
        fatal_error(errno, "sigaction");

    /* Initialize a sigevent structure to send a SIGUSR1 signal on timer
       expiration. */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo  = SIGUSR1;

    /* Create a timer with this sigevent structure.  */
    if (-1 == timer_create(CLOCK_MONOTONIC, &sev, &timerid) )
       fatal_error(errno, "timer_create");

    /* Set up the timer's initial value and repeat interval to be the same. */
    refresh_interval.it_value    = refresh_timespec;
    refresh_interval.it_interval = refresh_timespec;

    /* Arm the timer just created with the itimerspec interval. */
    if (-1 == timer_settime(timerid, 0, &refresh_interval, NULL) )
        fatal_error(errno, "timer_settime");

    get_window_size(0, &numrows, &numcols);
    setup_fd(STDIN_FILENO);
    write(STDOUT_FILENO, CLEAR_SCREEN, strlen(CLEAR_SCREEN));
    moveto(1,1);
    while( !finished  ) {
        if ( input_ready ) {
            input_ready = 0;
            user_row_adjust = 0;
            moveto(1,1);
            while ( read(STDIN_FILENO, &ch, 1)  > 0  && !finished ) {
                switch ( ch ) {
                case 'q': finished = TRUE; break;
                case 'd': user_row_adjust++;
                          break;
                case 'u': user_row_adjust--;
                          break;
                case '\n': continue;
                }
                moveto(2,1);
                write(1, CLEAR_ABOVE, clr_above_len);
                sprintf(msg, "\rYou entered %c\r", ch);
                moveto(numrows, 1);
                write(1, msg, strlen(msg));
                moveto(1, 1);
            }
        }
        if ( timer_expired ) {
            timer_expired = 0;
            oldcol = col;
            oldrow = row;
            row += user_row_adjust;
            if ( row < TOP_ROW )
                row = TOP_ROW;
            if ( row > numrows-1 )
                row = TOP_ROW;
            if ( col < numcols )
                col++;
            else {
                if ( row < numrows-1)
                    row++;
                else
                    row = TOP_ROW;
                col = 1;
            }
            moveto(oldrow, oldcol);
            write(1, &blank, 1);
            moveto(row, col);
            write(1, &sprite, 1);
            user_row_adjust = 0;
            moveto(1,1);
        }
        pause();
    }
    write(STDOUT_FILENO, CLEAR_SCREEN, strlen(CLEAR_SCREEN));
    return 0;
}
