/*****************************************************************************
  Title          : threaded_upcased.c
  Author         : Stewart Weiss
  Created on     : June 10, 2024
  Description    : A  program that shows effects of signals on threads
  Purpose        : To demonstrate rules of signals and threads
  Usage          : threaded_upcased
  Build with     : gcc -Wall -o threaded_upcased -I../include -L../lib \
                   threaded_upcased.c  -lspl -lm -lrt -pthread



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
#include <pthread.h>
#include "upcase.h"
#include  <syslog.h>
#include <ctype.h>

#define  MAXFD    64
#define  WARNING  "\nNOTE: SERVER NEVER  accessed private FIFO\n"
#define  MAXTRIES 10


int            dummyfd;        /* File descriptor for write-end of PUBLIC  */
int            publicfd;       /* File descriptor forread-end of PUBLIC     */
pid_t          server_pid;     /* Stores parent pid */
BOOL           is_daemon = FALSE;
/*****************************************************************************
                       Signal Handlers & Daemon Init
*****************************************************************************/


void on_sigpipe( int signo )
{
    error_mssge(-1, "Read end of pipe is not open.\n");
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
    pthread_exit(NULL);
}

void* sighandler( void * data)
{
    int  sig;
    sigset_t  mask = *(sigset_t*) data;

    if ( sigwait(&mask, &sig) != 0 )
          fatal_error(errno, "sigwait");

    close(dummyfd);
    close(publicfd);
    unlink(PUBLIC);
    exit(EXIT_SUCCESS);  /* Force all threads to exit. */

}

void* process_client( void *data )
{
    int  nbytes;
    int  tries;
    int  clientwritefd;
    int  clientreadfd;

    char *buffer = (char*) malloc(PIPE_BUF);
    if ( NULL == buffer )
        log_and_exit("malloc");

    message *client_msg = (message*) data;

    clientwritefd = -1;
    /* Client should have opened its rawtext_fd for writing before */
    /* sending the message, so the open here should succeed */
    if ( (clientwritefd = open(client_msg->raw_text_fifo, O_RDONLY)) == -1 )
        log_and_exit("Client did not open pipe for writing");

    /* Clear the buffer used for reading the client's text */
    memset(buffer, 0, PIPE_BUF);

        /* Set the locale. */
    if ( setlocale(LC_CTYPE, client_msg->locale)  == NULL )
           syslog(LOG_ERR, "Could not set the locale to %s",
           client_msg->locale);

    /* Attempt to read from client's raw_text_fifo;
       block waiting for input */
    while ( (nbytes = read(clientwritefd, buffer, PIPE_BUF)) > 0 ) {
        /* Convert the text to uppercase      */
        for ( int i = 0; i < nbytes; i++ )
            buffer[i] = toupper(buffer[i]);

        tries = 0;
        /* Open client's convertedtext_fd --  */
        /* Try 5 times or until client is reading */
        while (((clientreadfd = open(client_msg->upcased_fifo,
                 O_WRONLY | O_NONBLOCK)) == -1 ) && (tries < MAXTRIES ))
        {
             sleep(1);
             tries++;
        }
        if ( tries == MAXTRIES ) {
            /* Failed to open client private FIFO for writing */
            log_and_exit(WARNING);
        }

        /* Send converted text to client in its readfifo */
        if ( -1 == write(clientreadfd, buffer, nbytes) ) {
            if (  EPIPE == errno )
                log_and_exit("%m: Trying to write to client");
        }
        close(clientreadfd);  /* close write-end of private FIFO  */
        clientreadfd = -1;

        /* Clear the buffer used for reading the client's text */
        memset(buffer, 0, PIPE_BUF);
    }
    free(buffer);
    free(client_msg);
    pthread_exit(EXIT_SUCCESS);
}

/*****************************************************************************
                              Main Program
*****************************************************************************/


int main( int argc, char *argv[])
{
    message          msg;           /* Connection message                   */
    struct sigaction sigact;        /* sigaction for registering handlers   */
    sigset_t         mask;          /* Signal mask of blocked signals       */
    pthread_t        child_thread;  /* Thread ID for created child          */
    pthread_t        sig_thread;    /* Thread ID for signal-handling thread */
    pthread_attr_t attr;            /* Attribute structure for threads      */
    message          *client_data;  /* message structure to send to thread  */

    /* Create public FIFO */
    if ( mkfifo(PUBLIC, 0666) < 0 ) {
        if (errno != EEXIST ) {
            fprintf(stderr,"mkfifo() could not create %s\n", PUBLIC);
            exit(EXIT_FAILURE);
        }
        else
            fprintf(stderr,"%s exists. If server is not running,"
                           " delete and restart\n", PUBLIC);
    }
    /* Run this program as a daemon */
    make_me_a_daemon(argv[0]);
    is_daemon = TRUE;

    sigact.sa_handler = on_sigpipe;
    if ( sigaction(SIGPIPE, &sigact, NULL) == -1 )
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

    syslog(LOG_INFO, "Upcase server pid is %d\n", getpid());

    /* Set the attribute structure to create detached threads. */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize (&attr, 65536);

    /* Block likely asynchronous signals. */
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGABRT);
    if ( -1 == sigprocmask(SIG_BLOCK, &mask, NULL) )
        log_and_exit("sigprocmask failed");

    /* Create one thread to handle asynchronous terminating signals. */
    if ( 0 != pthread_create(&sig_thread, &attr, sighandler, (void*) &mask ) )
        syslog(LOG_ERR, "Could not create child thread.");


    /* Block waiting for a msg struct from a client */
    while ( read( publicfd, (char*) &msg, sizeof(msg)) > 0 ) {

        /* Allocate a new message structure for thread. */
        if ( NULL == (client_data = (message*) malloc(sizeof (message))) )
            log_and_exit("malloc");
        /* Copy the client message for passing to child thread. */
        memcpy(client_data, &msg, sizeof(msg));
        /* Create a new thread to process this client. */
        if ( 0 != pthread_create(&child_thread, &attr, process_client,
                                (void*) client_data ) )
            syslog(LOG_ERR, "Could not create child thread.");
        memset(&msg, 0, PIPE_BUF);
    }
    close(dummyfd);
    close(publicfd);
    unlink(PUBLIC);
    exit(EXIT_SUCCESS);
}


