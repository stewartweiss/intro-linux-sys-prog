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

  After you start the program, there are 3 characters that alter its behavior:
  d:  Moves the cursor down
  u:  Moves the cursor up
  q:  Exits the program
  Each must be followed by a newline to have any effect.

  This program will not behave "perfectly". If you type fast enough, you might
  see the characters typed in a row below the first row.If you enter a long
  sequence of characters before a newline, such as 'dddddd\n', you may see
  it briefly before it's erased.

  Because it's not disabling echoing of characters, these characters will be
  visible at the current cursor position. Therefore, it employs frequent
  movements of the cursor to position (1,1).

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

#define  FREQ_NS  100000000    /* Number of nanosecs in the  timer interval */
#define  TOP_ROW  2            /* Highest row in which sprite can be        */

volatile sig_atomic_t input_ready = 0;
volatile sig_atomic_t timer_expired = 0;

void home_cursor()
{
   write(STDOUT_FILENO, "\033[1;1H", 6);
}

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
    const char CLEAR_SCREEN[] = "\033[2J"; /* Escape seq to clear screen    */
    const char CLEAR_ABOVE[] =  "\033[1J"; /* Clears all lines above        */
    int   clr_above_len   = strlen(CLEAR_ABOVE); /* Length of CLEAR_ABOVE   */


    /* Get window dimensions and check that the window is large enough.     */
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
    if ( sigaction(SIGUSR1, &sigact, NULL) == -1 )  /* Try to install */
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
    home_cursor();
    while( !finished  ) {
    if ( input_ready ) {       /* SIGIO received. */
        input_ready = 0;       /* Reset flag.     */
        user_row_adjust = 0;   /* Net change in row position */
        home_cursor();
        while ( read(STDIN_FILENO, &ch, 1)  > 0  && !finished ) {
            switch ( ch ) {
            case 'q': finished = TRUE; break;  /* User wants to quit.       */
            case 'd': user_row_adjust++;       /* Increment adjustment.     */
                      break;
            case 'u': user_row_adjust--;       /* Decrement adjustmen.      */
                      break;
            case '\n': continue;               /* Ignore newline.           */
            }
            moveto(TOP_ROW-1,1);
            write(STDOUT_FILENO, CLEAR_ABOVE, clr_above_len); /* Clear line.*/
            sprintf(msg, "\rYou entered %c\r", ch); /* Format message.      */
            moveto(numrows, 1);                /* Move to bottom row.       */
            write(STDOUT_FILENO, msg, strlen(msg)); /* Print message.       */
            home_cursor();                     /* Home cursor at (1,1).     */
            }
        }
        if ( timer_expired ) {  /* Timer expiration. */
            timer_expired = 0;  /* Reset timer flag. */
            oldcol = col;       /* Save old position to replace.            */
            oldrow = row;       /* with space char.                         */
            row += user_row_adjust;  /* Adjust row by user's input.         */
            if ( row < TOP_ROW )     /* If above top row, move to top row.  */
                row = TOP_ROW;
            if ( row > numrows-1 )   /* Boundary conditions to check */
                row = TOP_ROW;
            if ( col < numcols )     /* Is it at rightmost column?   */
                col++;
            else {                   /* Yes - go to next row down.   */
                if ( row < numrows-1)/* OK to go down.               */
                    row++;
                else                 /* Not OK to go down. Start at top.    */
                    row = TOP_ROW;
                col = 1;
            }
            moveto(oldrow, oldcol); /* Get set to erase old sprite.         */
            write(STDOUT_FILENO, &blank, 1); /* Erase it.                   */
            moveto(row, col);       /* Move to new position to draw it.     */
            write(STDOUT_FILENO, &sprite, 1); /* Draw it.                   */
            user_row_adjust = 0;    /* Reset row adjustment to zero.        */
            home_cursor();          /* Home the cursor.                     */
        }
        pause();
    }
    write(STDOUT_FILENO, CLEAR_SCREEN, strlen(CLEAR_SCREEN));
    return 0;
}
