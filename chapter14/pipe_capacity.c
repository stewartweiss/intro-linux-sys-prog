/*****************************************************************************
  Title          : pipe_capacity.c
  Author         : Steart Weiss, based on a program by Haviland,Gray, & Salama
  Created on     : May 25, 2024
  Description    : Shows the maximum size of a pipe on the system
  Purpose        : To determine the maximum size of a pipe empirically by
                   writing until the buffer is full, and to demonstrate that
                   a write to the pipe cannot succeed unless the pipe has
                   at least PIPE_BUF free bytes.
  Usage          : pipe_capacity
  Build with     : gcc -Wall -I../include -L../lib -g -o pipe_capacity \
                          pipe_capacity.c -lspl

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
#include <limits.h>
#include <fcntl.h>



void set_non_blocking_mode(int fd)
{
    int flagset;

    flagset   = fcntl(fd, F_GETFL);
    flagset   |= O_NDELAY;
    fcntl(fd, F_SETFL, flagset);
}


int main(int argc, char* argv[])
{
    int fd[2];
    int pipe_size;
    int count = 0;
    if ( -1 == pipe(fd) )
        fatal_error(errno, "pipe failed");

    set_non_blocking_mode(fd[1]);
    pipe_size = fpathconf(fd[0],  _PC_PIPE_BUF);

    while ( TRUE ) {
        if ( ( write(fd[1], "0", 1) ) != 1) {
            if ( errno != EAGAIN )
                printf("errno = %d and write() failed\n", errno);
            break;
        }
        count++;
    }
    printf( "The maximum number of bytes that a pipe can store is %d.\n",
              count);
    printf( "The maximum size of an atomic write to a pipe is %d bytes\n",
            pipe_size);
    return 0;
}



