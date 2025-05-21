/*****************************************************************************
  Title          : upcase.c
  Author         : Stewart Weiss
  Created on     : May  2, 2008, revised Oct. 2024
  Description    : Client for upcase server daemon
  Purpose        : To demonstrate a multiple-client/server IPC using FIFOs
                   and to show how to use private and public FIFOs.
  Usage          : This client will send its standard input to the server
                   for translation to uppercase, one line at a time, whether
                   it comes from the console or is redirected. It also
                   accepts a command line argument
                       upcase   [input-file]
  Build with     : gcc -Wall -g -I../include -o upcase upcase.c -L../lib \
                       -lspl

*****************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include "upcase.h"     /* All required header files are included in this   */
                        /* header file shared by sender and receiver,       */

/****************************************************************************/
/*                           Defined Constants                              */
/****************************************************************************/


#define  MAXTRIES 5
const char server_no_read_msg[] = "The server is not reading the pipe.\n";

const char noserver_msg[] = "The server does not appear to be running. "
                               "Please start the service.\n";

const char missing_pipe_msg[] =
           "cannot communicate with the server due to a missing pipe.\n"
           "Check if the server is running and restart it if necessary.\n";

int      upcased_fd;      /* File descriptor for READ PRIVATE FIFO   */
int      rawtext_fd;      /* File descriptor to WRITE PRIVATE FIFO   */
int      publicfd;        /* File descriptor to write-end of PUBLIC  */
FILE*    inputfp;         /* File pointer to input stream            */
message  msg;             /* Connection message                      */

/****************************************************************************/
/*                          Utility Functions                               */
/****************************************************************************/

void clean_up()
{
    if ( upcased_fd != -1 )   close(upcased_fd);
    if ( rawtext_fd != -1 )   close(rawtext_fd);
    unlink(msg.upcased_fifo);
    unlink(msg.raw_text_fifo);
}


/****************************************************************************/
/*                            Signal Handlers                               */
/****************************************************************************/

void on_sigpipe( int signo )
{
    fprintf(stderr, "%s Exiting...\n", server_no_read_msg);
    unlink(msg.raw_text_fifo);
    unlink(msg.upcased_fifo);
    exit(EXIT_FAILURE);
}

void on_signal( int sig )
{
    if ( publicfd != -1 )     close(publicfd);
    if ( upcased_fd != -1 )   close(upcased_fd);
    if ( rawtext_fd != -1 )   close(rawtext_fd);
    unlink(msg.upcased_fifo);
    unlink(msg.raw_text_fifo);
    exit(EXIT_SUCCESS);
}
/****************************************************************************/
/*                              Main Program                                */
/****************************************************************************/

int main( int argc, char *argv[])
{
    int              strLength;     /* Number of bytes in text to convert   */
    int              nChunk;        /* Index of text chunk to send to server*/
    int              bytesRead;     /* Bytes received in read from server   */
    int              tries = 0;     /* Count of attempts to open FIFO       */
    static char      buffer[PIPE_BUF];
    static char      textbuf[BUFSIZ];
    struct sigaction sigact;

    if ( argc < 2)
        inputfp = stdin;
    else if ( NULL == (inputfp  = fopen(argv[1], "r")) )
        fatal_error(errno, argv[1]);

    publicfd = -1;
    upcased_fd   = -1;
    rawtext_fd = -1;

    /* Register the on_signal handler to handle all signals */
    sigact.sa_handler = on_signal;
    if ( ((sigaction(SIGINT, &sigact, NULL)) == -1 ) ||
         ((sigaction(SIGHUP, &sigact, NULL)) == -1 ) ||
         ((sigaction(SIGQUIT, &sigact, NULL)) == -1) ||
         ((sigaction(SIGTERM, &sigact, NULL)) == -1)
       )
        fatal_error(errno, "sigaction");

    sigact.sa_handler = on_sigpipe;
    if ( sigaction(SIGPIPE, &sigact, NULL) == -1 )
        fatal_error(errno, "sigaction");

    char* current_locale = setlocale(LC_CTYPE, "");
    if ( current_locale == NULL )
        fatal_error(errno, "setlocale");

    /* Create unique names for private FIFOs using process-id */
    sprintf(msg.upcased_fifo, "/tmp/fifo_rd%d",getpid());
    sprintf(msg.raw_text_fifo, "/tmp/fifo_wr%d",getpid());
    sprintf(msg.locale, "%s", current_locale);

    /* Create the private FIFOs */
    if ( mkfifo(msg.upcased_fifo, 0666) < 0 ||
         mkfifo(msg.raw_text_fifo, 0666) < 0 ) {
        clean_up();
        fatal_error(-1, "Error creating private FIFOs");;
    }

    /* Open the public FIFO for writing */
    if ( (publicfd = open(PUBLIC, O_WRONLY | O_NONBLOCK) ) == -1) {
        if ( ENXIO == errno )
            fprintf(stderr,"%s", noserver_msg);
        else if ( errno == ENOENT )
            fprintf(stderr,"%s %s", argv[0], missing_pipe_msg);
        else
            fprintf(stderr,"%d: ", errno);
        clean_up();
        exit(EXIT_FAILURE);
    }

    /* Send a message to server with names of two FIFOs */
    if ( write(publicfd, (char*) &msg, sizeof(msg)) == -1 )
        fatal_error(errno, "write to public fifo");

    /* Open the raw text fifo for writing: if server is not reading */
    /* try a few times and then exit */
    while (((rawtext_fd = open(msg.raw_text_fifo,
             O_WRONLY | O_NDELAY )) == -1 ) && (tries < MAXTRIES ))  {
            sleep(1);
            tries++;
    }
    if ( tries == MAXTRIES ) {
        /* Failed to open client private FIFO for writing */
        clean_up();
        fatal_error(-1,  server_no_read_msg);
    }

    /* Get one line of input at a time from the input source */
    while (TRUE) {
        memset(textbuf, 0, BUFSIZ);
        if ( NULL == fgets(textbuf, BUFSIZ, inputfp) )
            break;
        strLength = strlen(textbuf);


        /* Break input lines into chunks and send them one at a */
        /* time through the client's write FIFO */
        for ( nChunk = 0; nChunk < strLength; nChunk += PIPE_BUF-1 ) {
            memset(buffer, 0, PIPE_BUF);
            strncpy(buffer, textbuf+nChunk, PIPE_BUF-1);
            buffer[PIPE_BUF-1] = '\0';
            write(rawtext_fd, buffer, strlen(buffer));

            /* Open the private FIFO for reading to get output of command */
            /* from the server. */
            if ((upcased_fd = open(msg.upcased_fifo, O_RDONLY) ) == -1) {
                clean_up();
                fatal_error(errno, msg.upcased_fifo);
            }

            /* Read maximum number of bytes possible atomically */
            /* and copy them to standard output. */
            memset(buffer, 0, PIPE_BUF);
            while ((bytesRead= read(upcased_fd, buffer, PIPE_BUF)) > 0)
                write(fileno(stdout), buffer, bytesRead);

          close(upcased_fd);
          upcased_fd   = -1;
        }
    }
    clean_up();
    exit(EXIT_SUCCESS);
}




