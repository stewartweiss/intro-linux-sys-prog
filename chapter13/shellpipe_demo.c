/*****************************************************************************
  Title          : shellpipe_demo.c
  Author         : Stewart Weiss
  Created on     : May 16, 2024
  Description    : Simulates a 2-process shell pipe
  Purpose        : To show how to use dup2()
  Usage          : shellpipe_demo <cmmd> [options] [args] '|' \
                        cmmd [options] [args]
                   The | must be enclosed in quotes, otherwise the shell
                   will read it as a pipe for itself.
  Build with     : gcc -Wall -I../include -L../lib -g -o shellpipe_demo \
                          shellpipe_demo.c -lspl


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

int main(int argc, char* argv[])
{
    int    fd[2];
    int    i;
    int    pipe_index;      /* Index of '|' on command-line                 */
    char**  argvec;

    if ( argc < 3 )
        usage_error("Usage: shellpipe_demo command [options][args] '|'"
                    " command [options[ [args]\n ");

    if ( -1 == pipe(fd))
        fatal_error(errno, "pipe");

    /* Find index of '|' in the line, exit if it isn't found. */
    for ( i = 1; i < argc; i++ )
        if ( argv[i][0] == '|' )
            break;
    if ( i < argc )
        pipe_index = i;
    else
        usage_error("Usage: shellpipe_demo command [options][args] '|'"
                    " command [options[ [args]\n ");

    switch ( fork() ) {
    case -1:
        fatal_error(errno, "fork");
    case  0:       /* child1 */
        dup2(fd[WRITE_FD],fileno(stdout));   /* Now stdout points to fd[1]  */
        close(fd[READ_FD]);                  /* Close input end of pipe.    */
        close(fd[WRITE_FD]);                 /* Close output end of pipe.   */
        if ( NULL == (argvec = calloc(pipe_index, sizeof (char*))))
            fatal_error(errno, "calloc");

        for ( i = 0; i < pipe_index-1; i++ )
            argvec[i] = argv[i+1];
        execvp( argvec[0],argvec );          /* Run the first command.      */
        fatal_error(errno, "execvp");
    default:
        break;
    }

    switch ( fork()) {
    case -1:
        fatal_error(errno, "fork");
    case  0:
        dup2(fd[READ_FD], fileno(stdin));    /* Now stdin points to fd[0].  */
        close(fd[READ_FD]);                  /* Close input end of pipe.    */
        close(fd[WRITE_FD]);                 /* Close output end of pipe.   */

        if ( NULL == (argvec = calloc(argc-pipe_index, sizeof (char*))))
            fatal_error(errno, "calloc");

        for ( i = 0; i < argc- pipe_index; i++ )
            argvec[i] = argv[i+pipe_index+1];
        execvp( argvec[0], argvec );         /* Run the second command.     */
        fatal_error(errno, "execvp");
    default:
        close(fd[READ_FD]);  /*Parent closes its ends of the pipe.          */
        close(fd[WRITE_FD]);
        for ( i = 1; i <= 2; i++ )
            if ( wait(NULL) == -1)
                fatal_error(errno, "wait");
        exit(EXIT_SUCCESS);
    }
}

