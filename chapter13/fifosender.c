/*****************************************************************************
  Title          : fifosender.c
  Author         : Stewart Weiss
  Created on     : May 30, 2024
  Description/   : A simple FIFO client that sends to fifomonitor's FIFO
  Purpose        : To show how to use FIFOs for client-server-like model
  Usage          : fifosender
  Build with     : gcc -Wall -g -o fifosender -I../include fifosender.c \
                      -L../lib  -lspl

  Notes
  After invocation, enter text after the prompt; enter 'quit' to exit.


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

#include "fifodef.h"

#define  QUIT         "quit"

void on_sigpipe( int signo )
{
    fprintf(stderr, "Monitor is not running; terminating.\n");
    exit(1);
}


int main( int argc, char *argv[])
{
    int    nbytes;           /* Number of bytes read from standard input    */
    int    public_fd;        /* File descriptor to write end of PUBLIC FIFO */
    char   text[PIPE_BUF];   /* Buffer to store user entered text           */
    struct sigaction sigact; /* sigaction for registering handler           */
    char   usage[128];

    /* Only run if we are using the terminal. */
    if ( !isatty(fileno(stdin)) || !isatty(fileno(stdout)) ) {
        sprintf(usage,
        "%s\nMust be run without redirecting input or output.\n",  argv[0]);
        usage_error(usage);
    }


    /* Register the signal handler for SIGPIPE. */
    sigact.sa_handler = on_sigpipe;
    sigact.sa_flags = SA_RESTART;
    if ( (sigaction(SIGPIPE, &sigact, NULL)) == -1 )
            fatal_error(errno, "sigaction");

    /* Open the public FIFO for writing.                                    */
    if ( (public_fd = open(PUBLIC, O_WRONLY) ) == -1)
        fatal_error(errno, "open");

    printf("Type 'quit' to quit.\n");

    /* Repeatedly prompt user for input, read it, and send to server.     */
    while (TRUE) {
        memset(text, 0, PIPE_BUF);    /* Zero the buffer for the next read. */
        if ( (nbytes = read(STDIN_FILENO, text, PIPE_BUF)) == -1 )
            fatal_error(errno, "read");
        if ( !strncmp(QUIT, text, nbytes-1)) /* If text is 'quit', exit.    */
            break;
        if ( write(public_fd, text, nbytes) == -1 )
            break;  /* Might get SIGPIPE here, so no need to exit. */
    }
    /* User quit, so close the write-end of public FIFO */
    close(public_fd);
    return 0;
}



