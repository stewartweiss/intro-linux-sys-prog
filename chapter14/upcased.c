/*****************************************************************************
  Title          : upcased.c
  Author         : Stewart Weiss
  Created on     : June 2, 2024
  Description    : A concurrent lower-to-upper-case string server
  Purpose        : To demonstrate how to write a concurrent server for
                   multiple clients using the public/private FIFO paradigm.
  Usage          : upcased
  Build with     : gcc -Wall -g -I../include -L../lib -o upcased upcased.c \
                    -lspl -lrt

  Notes          : Start this server first using the command
                   upcased
                   Then start up any client as a foreground process.
                   Text entered at the client will be sent to this server
                   which will "upcase" it and send it back to the client.

                   You can run any number of clients. The server will remain
                   in the background unless you specifically kill it.
                   This server converts itself into a daemon using the method
                   described by Stevens in UNIX Network Programming.

                   This server handles all user-initiated terminating signals
                   by closing any descriptors that it has open and removing
                   the public FIFO and exiting. If it gets a SIGPIPE because
                   a client closed its read end of its private FIFO
                   immediately after sending a message but before the server
                   wrote back the converted string, it handles SIGPIPE by
                   continuing to listen for new messages and giving up on the
                   write to that pipe.

                   The server forks a process for each client that makes a
                   connection.

                   The server uses a waitpid() loop inside its SIGCHLD
                   handler to collect its zombie processes.

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


#include "upcase.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include  <syslog.h>
#include <ctype.h>

#define  MAXFD    64
#define  WARNING  "\nNOTE: SERVER ** NEVER ** accessed private FIFO\n"
#define  MAXTRIES 5

int            dummyfd;        /* File descriptor for write-end of PUBLIC  */
int            clientreadfd;   /* File descriptor for write-end of PRIVATE  */
int            clientwritefd;  /* File descriptor for write-end of PRIVATE  */
int            publicfd;       /* File descriptor forread-end of PUBLIC     */
pid_t          server_pid;     /* Stores parent pid */
BOOL           is_daemon = FALSE;  /* Am I a daemon?                        */

/****************************************************************************/
/*                     Signal Handlers & Daemon Init                        */
/****************************************************************************/


void on_sigchld( int signo )
{
    int   status;

    while ( waitpid(-1, &status, WNOHANG)  > 0 )
        continue;
    return;
}

void on_sigpipe( int signo )
{
    error_mssge(-1, "Read end of pipe is not open.\n");
}

void on_signal( int sig )
{
    close(dummyfd);
    if ( clientreadfd != -1 )
        close(clientreadfd);
    if ( clientwritefd != -1)
        close(clientwritefd);
    /* If this is the parent executing it, remove the public fifo */
    if ( getpid() == server_pid )
        unlink(PUBLIC);
    exit(EXIT_SUCCESS);
}


void make_me_a_daemon(const char *pname )
{
    int      max_descriptors;
    pid_t    pid;

    if ( (pid = fork()) == -1)
        fatal_error(errno, "fork");
    else if (pid != 0)
        exit(EXIT_SUCCESS); /* Parent terminates  */

    /* Child continues from here */
    setsid();            /* Detach itself and make itself a sesssion leader */
    signal(SIGHUP, SIG_IGN); /* Ignore SIGHUP */

    if ( (pid = fork()) == -1)
        fatal_error(errno, "fork");
    else if ( pid != 0 )
        exit(EXIT_SUCCESS);             /* First child terminates */

    /* Grandchild continues from here */
    chdir("/");             /* change working directory  */
    umask(0);               /* clear our file mode creation mask  */

    if ( -1 == (max_descriptors = sysconf(_SC_OPEN_MAX)) )
        max_descriptors = MAXFD;
    else
        for (int i = 0; i < max_descriptors; i++)
            close(i);  /* Close all open file descriptors */
    openlog(pname, LOG_PID | LOG_CONS, LOG_LOCAL0); /* Start syslog logging. */
}

void log_and_exit(char* errmssge)
{
    if ( is_daemon )
        syslog(LOG_ERR, "%s", errmssge);
    else
       error_mssge(-1, errmssge);
    exit(EXIT_FAILURE);
}

void process_client( message* msg)
{
    char    buffer[PIPE_BUF]; /* Buffer for reads                  */
    int     tries;         /* Number of tries to open private FIFO */
    int     nbytes;        /* Number of bytes read from FIFO       */

    clientwritefd = -1;
    /* Client should have opened its rawtext_fd for writing before */
    /* sending the message, so the open here should succeed */
    if ( (clientwritefd = open(msg->raw_text_fifo, O_RDONLY)) == -1 )
        log_and_exit("Client did not open pipe for writing");

    /* Clear the buffer used for reading the client's text */
    memset(buffer, 0, PIPE_BUF);

        /* Set the locale. */
    if ( setlocale(LC_CTYPE, msg->locale)  == NULL )
           syslog(LOG_ERR, "Could not set the locale to %s", msg->locale);

    /* Attempt to read from client's raw_text_fifo;
       block waiting for input */
    while ( (nbytes = read(clientwritefd, buffer, PIPE_BUF)) > 0 ) {
        /* Convert the text to uppercase      */
        for ( int i = 0; i < nbytes; i++ )
            buffer[i] = toupper(buffer[i]);

        tries = 0;
        /* Open client's convertedtext_fd --  */
        /* Try 5 times or until client is reading */
        while (((clientreadfd = open(msg->upcased_fifo,
            O_WRONLY | O_NONBLOCK)) == -1 ) && (tries < MAXTRIES ))  {
             sleep(1);
             tries++;
        }
        if ( tries == MAXTRIES ) {
            /* Failed to open client private FIFO for writing */
            log_and_exit(WARNING);
        }

        /* Send converted text to client in its readfifo */
        if ( (-1 == write(clientreadfd, buffer, nbytes)) && (EPIPE == errno) )
                log_and_exit("%m: Trying to write to client");

        close(clientreadfd);  /* close write-end of private FIFO  */
        clientreadfd = -1;

        /* Clear the buffer used for reading the client's text */
        memset(buffer, 0, PIPE_BUF);
    }
    exit(EXIT_SUCCESS);
}



/****************************************************************************/
/*                              Main Program                                */
/****************************************************************************/

int main( int argc, char *argv[])
{
    message          msg;           /* Connection message                   */
    struct sigaction sigact;        /* sigaction for registering handlers   */
    int              pid;

    /* Create public FIFO */
    if ( mkfifo(PUBLIC, 0666) < 0 ) {
        if (errno != EEXIST )
            fprintf(stderr,"mkfifo() could not create %s", PUBLIC);
        else
            fprintf(stderr,"%s already exists. Delete it and restart.\n",
                    PUBLIC);
        exit(EXIT_FAILURE);
    }

    /* Run this program as a daemon */
    make_me_a_daemon(argv[0]);
    is_daemon = TRUE;

    /* Register the signal handler  */
    sigemptyset(&sigact.sa_mask);
    sigact.sa_handler = on_signal;
    sigact.sa_flags = SA_RESTART;
    if ( ((sigaction(SIGINT,  &sigact, NULL)) == -1 ) ||
         ((sigaction(SIGHUP,  &sigact, NULL)) == -1 ) ||
         ((sigaction(SIGQUIT, &sigact, NULL)) == -1)  ||
         ((sigaction(SIGTERM, &sigact, NULL)) == -1)     )
        log_and_exit("sigaction failed");

    sigact.sa_handler = on_sigpipe;
    if ( sigaction(SIGPIPE, &sigact, NULL) == -1 )
        log_and_exit( "sigaction");

    sigact.sa_handler = on_sigchld;
    if ( sigaction(SIGCHLD, &sigact, NULL) == -1 )
        log_and_exit( "sigaction");



    /* Open public FIFO for reading and writing so that it does not get an */
    /* EOF on the read-end while waiting for a client to send data. */
    /* To prevent it from hanging on the open, the write-end is opened in  */
    /* non-blocking mode. It never writes to it. */
    if ( (publicfd = open(PUBLIC, O_RDONLY) ) == -1 ||
         ( dummyfd = open(PUBLIC, O_WRONLY | O_NONBLOCK )) == -1  ) {
        unlink(PUBLIC);
        log_and_exit("%m: Opening public fifos");
    }

    /* Store PID of parent process so  signal handler can compare  later.*/
    server_pid = getpid();
    syslog(LOG_INFO, "Server pid is %d\n", server_pid);

    /* Block waiting for a msg struct from a client */
    while ( read( publicfd, (char*) &msg, sizeof(msg)) > 0 ) {
        /* spawn child process to handle this client */
        if ( -1 == ( pid = fork() ) )
            syslog(LOG_ERR, "Could not create child process.");
        else if ( 0 == pid ) {
            /* fork a new process to service the incoming client
               request based on the private FIFO names in msg.*/
            process_client( &msg); /* Child process executes. */
            exit(EXIT_SUCCESS);   /* Should never get here! */
        }
    }
    unlink(PUBLIC);
    exit(EXIT_SUCCESS);
}


