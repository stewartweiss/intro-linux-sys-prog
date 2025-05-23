/*****************************************************************************
  Title          : nonblock_demo2.c
  Author         : Stewart Weiss
  Created on     : August 2, 2024
  Description    : Like nonblock_demo1, but overwrites lines to compact output
  Purpose        : Demonstrates use of  O_NONBLOCK flag for nonblocking input
  Usage          : nonblock_demo2 [delay-usecs]
  Build with     : gcc -Wall -g -I../include -L../lib -o nonblock_demo2 \
                   nonblock_demo2.c -lspl

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


void set_non_block(int fd )
{
    int flagset;

    flagset   = fcntl(fd, F_GETFL);
    if ( flagset == -1 )
        fatal_error(errno, "fcntl");
    flagset   |= O_NONBLOCK;
    if ( -1 == fcntl(fd, F_SETFL, flagset))
        fatal_error(errno, "fcntl");
}

void moveto(int line, int col )
{
    char seq_str[20];
    sprintf(seq_str, "\033[%d;%dH", line, col);
    write(STDOUT_FILENO, seq_str, strlen(seq_str));
}

int main (int argc, char *argv[])
{
    char   ch;
    int    count = 0;
    int    done  = 0;                /* To control when to stop loop */
    char   str[128];
    const  char CLEAR_SCREEN[] = "\033[2J";
    const  char CLEAR_LINE[]   =  "\033[1A\033[2K\033[G";
    set_non_block( STDIN_FILENO );   /* Turn off blocking mode. */
    write(STDOUT_FILENO,CLEAR_SCREEN, strlen(CLEAR_SCREEN));
    moveto(1,1);
    while ( !done  ) {
        count++;
        usleep(500000);  /* Delay a bit. */
        /* Do the read. If nothing was typed, print a message. */
        write(STDOUT_FILENO,CLEAR_LINE, strlen(CLEAR_LINE));
        if ( read(STDIN_FILENO, &ch, 1) > 0 ) {
            if ( ch == 'q' )
                done = 1;
            else if ( ch != '\n') {
                moveto(2,1);
                sprintf(str, "\rUser entered %c\n", ch);
                write(1, str, strlen(str));
                moveto(1,1);
            }
        }
        else {
            moveto(3,1);
            sprintf(str, "\rNo input; number of unsatisfied reads = %d\n",
                    count);
            write(1, str, strlen(str));
            moveto(1,1);

        }
    }
    write(STDOUT_FILENO,CLEAR_SCREEN, strlen(CLEAR_SCREEN));
    return 0;
}





