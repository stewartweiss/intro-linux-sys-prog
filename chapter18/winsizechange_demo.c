/*****************************************************************************
  Title          : winsizechange_demo.c
  Author         : Stewart Weiss
  Created on     : June 20, 2024
  Description    : Draws dots to fill screen, responds to window size change
  Purpose        : To show how to handle window resize events
  Usage          : winsizechange_demo
  Build with     : gcc -Wall -g -o winsizechange_demo -I../include -L../lib\
                   winsizechange_demo.c -lrt -lspl

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
#include <ctype.h>

#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

int fd;
int numrows;
int numcols;

void winsize_handler( int signo )
{
    struct winsize size;

    if (ioctl(fd, TIOCGWINSZ,  &size) < 0) {
        perror("TIOCGWINSZ error");
        return;
    }
    numrows = size.ws_row;
    numcols = size.ws_col;
}

/****************************************************************************/

void  get_window_size( int fd, int *rows, int *cols )
{
    struct winsize size;

    if (ioctl(fd, TIOCGWINSZ,  &size) < 0) {
        perror("TIOCGWINSZ error");
        return;
    }
    *rows = size.ws_row;
    *cols = size.ws_col;
}

/****************************************************************************/
void modify_termios(int fd, int echo, int canon )
{
    struct termios cur_tty;
    tcgetattr(fd, &cur_tty);

    if ( canon )
        cur_tty.c_lflag     |= ICANON;
    else
        cur_tty.c_lflag     &= ~ICANON;
    if ( echo )
        cur_tty.c_lflag     |= ECHO;
    else
        cur_tty.c_lflag     &= ~ECHO;
    cur_tty.c_lflag     &= ~ISIG;
    cur_tty.c_cc[VMIN]   =  1;
    cur_tty.c_cc[VTIME]  =  0;
    tcsetattr(fd, TCSADRAIN, &cur_tty);
}


void set_non_block(int fd )
{
    int flagset;

    flagset   = fcntl(fd, F_GETFL);
    flagset   |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flagset);
}


int main (int argc, char *argv[])
{
    char     ch;         // stores user's char
    char   period = '.';
    size_t bytecount;
    int    count = 0;
    int    done  = 0;   /* to control when to stop loop */
    int    pause = 0;   /* to control pausing of output */
    struct sigaction sa;

    /* Check whether input or output has been redirected */
    if ( !isatty(0) || !isatty(1) ) {
        usage_error("Do not run this program with redirected I/O");
    }

    get_window_size(1, &numrows, &numcols);

    /* Turn off blocking mode */
    set_non_block( STDIN_FILENO );
    modify_termios( STDIN_FILENO, 1, 0);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = winsize_handler;
    sigaction(SIGWINCH, &sa, NULL);

    /* Start drawing. Stop when the screen is full */
    while ( !done  ) {
        if ( ! pause ) {
            count++;
            /* Is screen full except for bottom row? */
            if ( count > (numcols * (numrows-1)) ) {
                pause = 1;
                count--;
            }
            else
                write(STDOUT_FILENO, &period, 1);
        }
        usleep(10000);  /* delay a bit */

        if ( (bytecount = read(STDIN_FILENO, &ch, 1) ) > 0 ) {
            if ( ch == 'q' )
                done = 1;
            else if ( ch == 'p' )
                pause = 1;

            else if ( ch == 'r' )
                pause = 0;
        }
    }
    write(1, "\033[2J", 4);
    write(1, "\033[3J", 4);
    return 0;
}
