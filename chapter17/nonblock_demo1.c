/*****************************************************************************
  Title          : nonblock_demo1.c
  Author         : Stewart Weiss
  Created on     : August 2, 2024
  Description    :
  Purpose        : Demonstrates use of  O_NONBLOCK flag for nonblocking input
  Usage          : nonblock_demo1 [delay-usecs]
  Build with     : gcc -Wall -g -I../include -L../lib -o nonblock_demo1 \
                   nonblock_demo1.c -lspl

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

int main (int argc, char *argv[])
{
    char   ch;
    int    count = 0, failedcount = 0;
    char   str[128];
    long   delay = 500000;

    if ( argc > 1 )
        delay = strtol(argv[1], NULL, 0);

    set_non_block( STDIN_FILENO );   /* Turn off blocking mode.     */
    while ( count < 500  ) {
        count++;
        if ( -1 == usleep(delay))    /* Delay a bit.                */
            fatal_error(errno, "usleep");
        if ( read(STDIN_FILENO, &ch, 1) > 0 ) {
            if ( ch == 'q' )
                break;
            else if ( ch != '\n') {  /* Don't print entered newline. */
                sprintf(str, "\rUser entered %c\n", ch);
                write(1, str, strlen(str));
            }
        }
        else { /* read() returned -1, implying no data available.    */
            failedcount++;                     /* Update counter.             */
            sprintf(str, "\rNo input; number of unsatisfied reads = %d\n",
                    failedcount);
            write(1, str, strlen(str));
        }
    }
    return 0;
}





