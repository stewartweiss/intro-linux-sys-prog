/*****************************************************************************
  Title          : pipe_demo1.c
  Author         : Stewart Weiss
  Created on     : May 16, 2024
  Description    : Demonstrates a pipe between parent and child.
  Purpose        : To introduce the pipe() system call.
  Usage          : pipe_demo1 <string>
                   To send multiple words, enclose in single quotes
  Build with     : gcc -Wall -I../include -L../lib -g -o pipe_demo1 \
                          pipe_demo1.c -lspl


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

#define  READ_FD  0         /* Make it easier to see the read end.          */
#define  WRITE_FD 1         /* Make it easier to see the write end.         */

int main(int argc, char *argv[])
{
    int  pipefd[2];        /* The array of file descriptors for pipe()      */
    char buffer;           /* A single char to receive and print            */

    if (argc < 2)
        usage_error("Usage: pipe_demo0 <arg> ");

    if (pipe(pipefd) == -1) /* Create the pipe. Must be done before fork(). */
        fatal_error(errno, "pipe");

    switch ( fork() ) {
    case -1:
        fatal_error(errno, "fork()");

    case 0:    /* Child code */
        char  label[] = "Child received: ";
        close(pipefd[WRITE_FD]); /* MUST DO THIS otherwise child will       */
                                 /* never terminate!                        */
        write(STDOUT_FILENO, &label, strlen(label));
        /* Loop while not EOF and not a read error.                         */
        while ( ( read( pipefd[READ_FD], &buffer, 1) ) > 0 )
            write(STDOUT_FILENO, &buffer, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[READ_FD]);
        exit(EXIT_SUCCESS);

    default:                      /* Parent code                            */
        close(pipefd[READ_FD]);   /* Parent is writing, so close read end.  */
        write(pipefd[WRITE_FD], argv[1], strlen(argv[1]));
        close(pipefd[WRITE_FD]);  /* Reader will see EOF.                   */

        if ( wait(NULL) == -1 )   /* Wait for the child.                    */
            fatal_error(errno, "wait");
        exit(EXIT_SUCCESS);
   }
}
