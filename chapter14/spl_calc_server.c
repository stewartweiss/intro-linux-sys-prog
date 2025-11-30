/*****************************************************************************
  Title          : spl_calc_server.c
  Author         : Stewart Weiss
  Created on     : May 16, 2024
  Description    : Server that evaluates bc arithmetic expressions
  Purpose        : To introduce iterative server and client design
  Usage          : spl_calcd &
  Build with     : gcc -Wall -I../include -L../lib -g -o spl_calcd \
                          spl_calc_server.c -lspl

  Usage          : Start this server first using the command
                   spl_calcd &
                   Then start up any client as a foreground process.
                   Expressions sent by the client will be evaluated and
                   sent back to the client.

  Comments       : This server handles all user-initiated terminating signals
                   by closing any descriptors that it has open and removing
                   the public FIFO and exiting. If it gets a SIGPIPE because
                   a client closed its read end of its private FIFO
                   immediately after sending a message but before the server
                   wrote back the converted string, it handles SIGPIPE by
                   continuing to listen for new messages and giving up on the
                   write to that pipe.

  Modifications:   11/30/2025 by SNW: I moved the call to unlink the public
                   FIFO in the signal handler to precede the test of privatefd
                   there. This guarantees that the FIFO is removed even if the
                   signal hqndler runs after privatefd was closed bu before it
                   was set to -1.

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
#include "spl_calc.h"

#define  WARNING  "\nServer could not access client's private FIFO\n"
#define  MAXTRIES 5

int            dummyfd;       /* File descriptor to write-end of PUBLIC */
int            publicfd;      /* File descriptor to read-end of PUBLIC */
int            privatefd;     /* File descriptor to write-end of PRIVATE */

/*****************************************************************************/
/*                            Signal Handlers                                */
/*****************************************************************************/

void on_sigpipe( int signo )
{
    fprintf(stderr, "Client is not reading the pipe.\n");
}

void on_signal( int signo )
{
    close(publicfd);
    close(dummyfd);
    unlink(PUBLIC);
    if ( privatefd != -1 )
        close(privatefd);
    exit(0);
}

/*****************************************************************************/
/*                              Main Program                                 */
/*****************************************************************************/

int main( int argc, char *argv[])
{
    int              tries;         /* Number of tries to open private FIFO */
    int              nbytes;        /* Number of bytes read from popen()    */
    int              done;          /* Flag to stop loop                    */
    struct message   msg;           /* Private fifo name and command        */
    struct sigaction sigact;        /* sigaction for registering handlers   */
    char             result[PIPE_BUF+1];  /* Result to return to client     */
    char             command[TEXTLEN+32]; /* Command for popen() to execute */
    FILE*            fp;            /* FILE stream to read end of popen()   */


    /* Register the signal handlers.  */
    sigact.sa_handler = on_signal;
    sigact.sa_flags = SA_RESTART;
    if ( ((sigaction(SIGINT, &sigact, NULL)) == -1 ) ||
         ((sigaction(SIGHUP, &sigact, NULL)) == -1 ) ||
         ((sigaction(SIGQUIT, &sigact, NULL)) == -1) ||
         ((sigaction(SIGTERM, &sigact, NULL)) == -1)
       )
          fatal_error(errno, "sigaction");

    sigact.sa_handler = on_sigpipe;
    if ( sigaction(SIGPIPE, &sigact, NULL) == -1 )
            fatal_error(errno, "sigaction");

    /* Create public FIFO. */
    if ( mkfifo(PUBLIC, 0666) < 0 ) {
        if (errno != EEXIST )
            fatal_error(errno, "mkfifo");
        else
            fprintf(stderr,"%s already exists. Delete it and restart.\n",
                    PUBLIC);
        exit(EXIT_FAILURE);
    }

    /* Open public FIFO for reading and writing so that it does not get an  */
    /* EOF on the read-end while waiting for a client to send data.         */
    /* To prevent it from hanging on the open, the write-end is opened in   */
    /* non-blocking mode. It never writes to it.                            */

    if ( (publicfd = open(PUBLIC, O_RDONLY) ) == -1 )
        fatal_error(errno, "open");
    if ( (dummyfd = open(PUBLIC, O_WRONLY | O_NONBLOCK ))  == -1  )
        fatal_error(errno, "open");

    /* Block while waiting for a msg struct from a client */
    while ( read( publicfd, (char*) &msg, sizeof(msg)) > 0 ) {
        /* A msg arrived; start trying to open write end of private FIFO.   */
        tries = done = 0;
        privatefd    = -1;
        do {
            if ((privatefd = open(msg.fifo_name, O_WRONLY | O_NONBLOCK)) < 0 )
                sleep(1);   /* sleep if failed to open */
            else { /* Create command to give to popen(). */
                memset(command, 0, strlen(command)); /* Clear command. */
                sprintf(command, "echo \"%s\" | bc  ",msg.text );
                if ( (fp = popen(command, "r") ) == NULL )
                    fatal_error(errno,"popen");
                memset(result, 0, PIPE_BUF);
                nbytes = read(fileno(fp), result, PIPE_BUF);
                if ( -1 == nbytes )
                    error_mssge(errno, "Read from bc");
                else if ( 0 == nbytes )
                    error_mssge(errno, "Null output from bc");
                else {  /* Send  result to client.  */
                    result[nbytes] = '\0'; /* Null-terminate. */
                    if ( -1 == write(privatefd, result, nbytes+1) )
                        error_mssge(errno,"write to client fifo");
                }
                pclose(fp);        /* Wait for popen status. */
                close(privatefd);  /* Close write-end of private FIFO */
                done = 1;          /* Terminate loop */
            }
        } while (++tries < MAXTRIES && !done);

        if ( !done)
            /* Failed to open client private FIFO for writing */
            write(fileno(stderr), WARNING, sizeof(WARNING));
    }
    return 0;
}

