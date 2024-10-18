/*****************************************************************************
  Title          : pipe_demo2.c
  Author         : Stewart Weiss
  Created on     : April 29, 2024
  Description    : Simulates a producer/consumer program with 2 processes
  Purpose        :
  Usage          : pipe_demo2
  Build with     : gcc -Wall -I../include -L../lib -g -o pipe_demo2 \
                          pipe_demo2.c -lspl

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
#include <sys/wait.h>
#include <ctype.h>

#define  READ_FD  0         /* Make it easier to see the read end.          */
#define  WRITE_FD 1         /* Make it easier to see the write end.         */


void producer(int fd[] )
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    if ( close(fd[READ_FD]) == -1 )
        fatal_error(errno, "close");
    while ((nread = getline(&line, &len, stdin)) != -1)
        write(fd[WRITE_FD], line, nread);

    free(line);
    close(fd[WRITE_FD]);
    exit(EXIT_SUCCESS);
}

void consumer(int fd[] )
{
    char buffer, ch;

    if ( close(fd[WRITE_FD]) == -1 )
        fatal_error(errno, "close");
    /* Loop while not EOF and not a read error.          */
    while (  read( fd[READ_FD], &buffer, 1)  > 0 ) {
        ch = toupper(buffer);
        write(STDOUT_FILENO, &ch, 1);
    }
    if ( close(fd[READ_FD]) == -1 )
        fatal_error(errno, "close");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    int  pipefd[2];            /* The array of file descriptors for pipe()  */

    if (pipe(pipefd) == -1)    /* Create the pipe.                          */
        fatal_error(errno, "pipe");

    /* Create the producer process.                                         */
    switch ( fork()) {
    case -1:
        fatal_error(errno, "fork");
    case 0:
        producer(pipefd);
    default:   break;
    }

    /* Create the consumer process.                                         */
    switch ( fork()) {
    case -1:
        fatal_error(errno, "fork");
    case 0:
        consumer(pipefd);
    default: break;
    }

    /* Close both ends of the pipe.                                         */
    close(pipefd[READ_FD]);
    close(pipefd[WRITE_FD]);

    /* Wait for the children to terminate.                                  */
    for (int i = 0; i < 2; i++ )
        if ( wait( NULL) == -1 )
             break;
    exit(EXIT_SUCCESS);
}

