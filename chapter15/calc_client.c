/*****************************************************************************
  Title          : spl_calc_client.c
  Author         : Stewart Weiss
  Created on     : May 16, 2024
  Description    : Client for spl_calc  daemon
  Purpose        : To introduce iterative server and client design
  Usage          : spl_calc '<string>'
  Build with     : gcc -Wall -I../include -L../lib -g -o spl_calc \
                          spl_calc_client.c -lspl

  Usage          : Make sure the server is running in the background before
                   running the client:
                       spl_calcd  &
                  Then start up the client as a foreground process in any
                  terminal window.
                  Provide a command line argument enclosed in single quotes
                  containing any valid arithmetic expression compatible with
                  the   POSIX bc specification


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
#define _GNU_SOURCE
#include "spl_calc.h"   /* All required header files are included in this */
                        /* header file shared by sender and receiver,  */

/****************************************************************************/
/*                           Defined Constants                              */
/****************************************************************************/

const char   startup_msg[] =
               "calcd server does not seem to be running. "
               "Please start the service by entering 'calcd &'\n";

volatile sig_atomic_t  sig_received = 0;
struct message   msg;
int              private_fd; /* File descriptor to read-end of PRIVATE */
int              public_fd;  /* File descriptor to write-end of PUBLIC */


/*****************************************************************************/
/*                            Signal Handlers                                */
/*****************************************************************************/

void on_sigpipe( int signo )
{
    fprintf(stderr, "calcd is not reading the FIFO.\n");
    unlink(msg.fifo_name);
    exit(1);
}

void on_signal( int signo )
{
    close(private_fd);
    close(public_fd);
    unlink(msg.fifo_name);
}
/****************************************************************************/
/*                              Main Program                                */
/****************************************************************************/

int main( int argc, char *argv[])
{
    int              bytesRead;   /* Bytes received in read from server     */
    static char      buf[PIPE_BUF]; /* Buffer to store returned data        */
    char             usage[NAME_MAX]; /* Usage message                      */
    struct sigaction handler;
    char             label[]="CALC RESULT:";

    if ( argc < 2 ) {
       sprintf(usage, "%s <expression>", basename(argv[0]));
       usage_error(usage);
       /* Could read from standard input instead in this case. */
    }
    memset(msg.text, 0, TEXTLEN);
    if ( strlen(argv[1]) >= TEXTLEN )
        fatal_error(-1, "Expression too long");
    strcpy(msg.text, argv[1]);

    /* Create unique name for private FIFO using process-id */
    sprintf(msg.fifo_name, "/tmp/fifo%d",getpid());

    /* Create the private FIFO  */
    if ( mkfifo(msg.fifo_name, 0666) < 0 )
        fatal_error(errno,msg.fifo_name);

    /* Register the on_signal handler to handle all signals */
    handler.sa_handler = on_signal;      /* handler function    */
    if ( ((sigaction(SIGINT, &handler, NULL)) == -1 ) ||
         ((sigaction(SIGHUP, &handler, NULL)) == -1 ) ||
         ((sigaction(SIGQUIT, &handler, NULL)) == -1) ||
         ((sigaction(SIGTERM, &handler, NULL)) == -1)
       )
            fatal_error(errno, "sigaction");

    handler.sa_handler = on_sigpipe;
    if ( sigaction(SIGPIPE, &handler, NULL) == -1 )
        fatal_error(errno, "sigaction");

    /* Open the public FIFO for writing */
    if ( (public_fd = open(PUBLIC, O_WRONLY | O_NONBLOCK) ) == -1) {
        if ( ENXIO == errno )
            fprintf(stderr,"%s", startup_msg);
        else
            error_mssge(errno,PUBLIC);
        exit(EXIT_FAILURE);
    }

    /* Display label for returned upper case text */
    write(fileno(stdout), label, strlen(label));

    write(public_fd, (char*) &msg, sizeof(msg));

    /* Open the private FIFO for reading to get output of command */
    /* from the server. */
    if ((private_fd = open(msg.fifo_name, O_RDONLY) ) == -1)
        fatal_error(errno, msg.fifo_name);

    /* Read maximum number of bytes possible atomically */
    /* and copy them to standard output. */
    while ((bytesRead= read(private_fd, buf, PIPE_BUF)) > 0)
        write(fileno(stdout), buf, bytesRead);

    close(private_fd);      /* close the read-end of private FIFO */

    /* User quit, so close write-end of public FIFO and delete private FIFO */
    close(public_fd);
    unlink(msg.fifo_name);
    return 0;
}



