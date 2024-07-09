/*****************************************************************************
  Title          : fifomonitor.c
  Author         : Stewart Weiss
  Created on     : May 30, 2024
  Description/   : A simple FIFO server that prints messages from clients.
  Purpose        : To show how to use FIFOs for client-server-like model
  Usage          : fifomonitor &
  Build with     : gcc -Wall -g -o fifomonitor -I../include fifomonitor.c \
                      -L../lib  -lspl

  Notes
  To terminate this server, send it any terminating signal other than SIGKILL


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

/* These are global so that the signal handler and main can access them.    */
int    public_fd;    /* File descriptor to read-end of PUBLIC fifo          */
int    dummy_fd;     /* File descriptor to unused write-end of PUBLIC fifo  */

/****************************************************************************/
/*                             Signal Handler                               */
/****************************************************************************/

/** clean_up()
 *  This closes both ends of the FIFO and then removes it, after which it
 *  exits the program.
 */
void clean_up( int sig )
{
    close(public_fd);
    close(dummy_fd);
    unlink(PUBLIC);
    exit(sig);
}

/****************************************************************************/
/*                              Main Program                                */
/****************************************************************************/

int main( int argc, char *argv[])
{
    int              nbytes;          /* Number of bytes read from client   */
    int              msg_count = 0;   /* Number of messages received        */
    static char      buffer[PIPE_BUF+1];   /* Buffer for received message   */
    struct sigaction handler;         /* sigaction for registering handlers */
    struct timespec  tp;
    struct tm        *bdtime;
    char             timestr[64];
    char             msgnum[16];


    /* Register the signal handler to handle termination signals.           */
    handler.sa_handler = clean_up;
    handler.sa_flags = 0;
    sigemptyset(&handler.sa_mask);
    if ( ((sigaction(SIGINT,  &handler, NULL)) == -1 ) ||
         ((sigaction(SIGHUP,  &handler, NULL)) == -1 ) ||
         ((sigaction(SIGQUIT, &handler, NULL)) == -1)  ||
         ((sigaction(SIGTERM, &handler, NULL)) == -1)     )
        fatal_error(errno, "sigaction");


    /* Create public FIFO. If it exists already, the call will return -1 and
       set errno to EEXIST. This is not an error in our case. It just means
       we can reuse an existing FIFO that we created but never removed. All
       other errors cause the program to exit.
    */
    if ( mkfifo(PUBLIC, 0666) == -1 )
        if (errno != EEXIST )
            fatal_error(errno, "open");

    /*
      Open the FIFO for reading, with the O_NONBLOCK flag clear. The POSIX
      semantics state the the process will be blocked on the open() until some
      process opens it for writing. Therefore, the monitor will block in this
      open() until a client starts up.
    */
    if ( (public_fd = open(PUBLIC, O_RDONLY) ) == -1 )
        fatal_error(errno, "open");

    /*
      Open the FIFO for writing, even though we have no intention of writing
      to the FIFO in the monitor. This is because the POSIX semantics of
      read() is that read() will return 0 if no process has the FIFO open for
      writing.  We will not reach the call to open() until a client runs,
      but once the client runs, the monitor opens the FIFO for writing.
      Without this, when the client terminates and closes its write-end of the
      FIFO, the monitor's read loop would exit and the monitor would also exit.
      This "dummy" open keeps the monitor alive.
    */
    if ( (dummy_fd = open(PUBLIC, O_WRONLY )) == -1  )
        fatal_error(errno, "open");

    /* Repeatedly do a blocking read, waiting for a message from a client.  */
    while ( TRUE ) {
        memset(buffer, 0, PIPE_BUF);  /* Zero the buffer for the next read. */
        if ( ( nbytes = read( public_fd, buffer, PIPE_BUF)) > 0 ) {
            buffer[nbytes] = '\0';  /* Add NULL byte to print it.           */

            /* Get the current time.   */
            if ( -1 == clock_gettime(CLOCK_REALTIME, &tp) )
                fatal_error(errno, "clock_gettime()");

            /* Convert the current time into broken-down time. */
            bdtime = localtime(&tp.tv_sec);
            if (bdtime == NULL)
                fatal_error(EOVERFLOW, "localtime");
            strftime(timestr, sizeof(timestr),"%X", bdtime);
            sprintf(msgnum, "[%d]", ++msg_count);
            printf("%-5s %s: %s", msgnum, timestr, buffer);
            fflush(stdout);  /* In case there's no newline.                 */
        }
        else {
             printf("No bytes left in buffer\n");
             break;
        }
    }
    exit(EXIT_SUCCESS);
}





