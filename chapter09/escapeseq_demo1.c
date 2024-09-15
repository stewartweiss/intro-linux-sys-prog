/*****************************************************************************
  Title          : escapeseq_demo1.c
  Author         : Stewart Weiss
  Created on     : January  2, 2024
  Description    : Demonstrates use of a few ANSI escape sequences
  Purpose        : To show how to use  ANSI escape sequences
  Usage          : escapeseq_demo1
  Build with     : gcc -Wall -g -I../include -L../lib -o escapeseq_demo1 \
                   escapeseq_demo1.c -lspl

  Notes          :
  Every ANSI escape sequence begins with the character whose code is
  decimal code 27, or octal 033, which we will write as ESC. Many codes
  begin with a two-character sequence consisting of ESC followed by '[',
  although there are also escape sequences that use just the ESC.

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

#define _GNU_SOURCE
#include "common_hdrs.h"
#include <termios.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#define CLEAR_SCREEN    "\033[2J"
#define CURSOR_HOME     "\033[1;1H"


/** get_winsize(fd,&r, &c) stores window dimensions in r and c. */
void get_winsize(int fd, unsigned short *rows, unsigned short *cols )
{
    struct winsize size;

    if (ioctl(fd, TIOCGWINSZ,  &size) < 0) {
        perror("TIOCGWINSZ error");
        return;
    }
    *rows = size.ws_row;
    *cols = size.ws_col;
}

/* moveto(row,col) moves cursor to position (row,col) (origin is (0,0).   */
void moveto(int row, int col )
{
    char seq_str[20];

    sprintf(seq_str, "\033[%d;%dH", row, col);
    write(STDOUT_FILENO, seq_str, strlen(seq_str));
}


int main (int argc, char *argv[])
{
    int    i;
    char*  line;
    unsigned short    nrows, ncols;

    // This is how to test whether output has been redirected:
    if ( !isatty(fileno(stdout)) )
        fatal_error(errno, "isatty");

    line = calloc(512, sizeof(char));
    if ( NULL == line  )
        exit(EXIT_FAILURE);

    get_winsize(1, &nrows, &ncols);  // Get rows and columns

    // Clear the screen.
    sprintf(line,  "%s%s", CLEAR_SCREEN,CURSOR_HOME);
    write(1,line, strlen(line));
    for ( i = 1; i <= nrows ; i += 1) {
        sprintf(line, "\033[%d;%dH+", i, i);
        write(1,line, strlen(line));
        usleep(300000);
    }
    sprintf(line,  "%s%s", CLEAR_SCREEN,CURSOR_HOME);
    write(1,line, strlen(line));
    free(line);
    return (0);
}





