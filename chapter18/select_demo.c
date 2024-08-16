/*****************************************************************************
  Title          : select_demo.c
  Author         : Stewart Weiss
  Created on     : August 14, 2024
  Description    : A simulation of a server-like program using select()
  Purpose        : To show how to use select()
  Usage          :  select_demo
  Build with     : gcc -Wall -g -o select_demo -I../include -L../lib \
                   select_demo.c -lspl -lrt -lm

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
#include "time_utils.h"
#include <sys/select.h>
#include <sys/wait.h>

#define  READ_END    0
#define  WRITE_END   1
#define  RECV_DELAY  4
#define  NSENDERS    6

/* The structure of messages sent by sender processes to the parent process */
typedef struct _msg {
    struct timespec gen_time;  /* Time at which message was generated */
    char   content[32];        /* Text string sent by sender process   */
} message;

/** print_time(ts, 0, str) formats the time in timespec ts as a string in str
 *  but print_time(ts, 1, str) formats the time ignoring timezone information
 */
void print_time(struct timespec time_field, int format_flag, char *str)
{
    struct tm *bdtime;                 /* Broken-down time                  */
    char       timestr[100];           /* String storing formatted time     */
    time_t     time_val;               /* For converted tv_sec field        */

    time_val = time_field.tv_sec;
    if (format_flag == 0 ) {           /* Format as calendar time.          */
        if ( (bdtime = localtime(&time_val)) == NULL )
            fatal_error(EOVERFLOW, "localtime");
        if ( strftime(timestr, sizeof(timestr), "%X", bdtime) == 0 )
            fatal_error(BAD_FORMAT_ERROR,"strftime failed\n");
    }
    else {                /* Format using gmtime() to get UTC time.         */
        if ( (bdtime = gmtime(&time_val)) == NULL )
            fatal_error(EOVERFLOW, "gmtime");
        if ( strftime(timestr, sizeof(timestr), "%H:%M:%S", bdtime) == 0 )
            fatal_error(BAD_FORMAT_ERROR,"strftime failed\n");
    }
    sprintf(str, "%s.%09lu", timestr, time_field.tv_nsec );
}

int fillpipe( int pipefd )
{
    int count = 0;
    int flagset;
    int nbytes = 0;

    char fill[PIPE_BUF];
    memset(fill, '0', PIPE_BUF);

    flagset   = fcntl(pipefd, F_GETFL);
    fcntl(pipefd, F_SETFL, flagset | O_NONBLOCK);
    while ( TRUE ) {
        if ( (nbytes = write(pipefd, fill, PIPE_BUF) ) <= 0) {
            if ( errno != EAGAIN )
                fatal_error(errno, "write to pipe");
            break;
        }
        count += nbytes;
    }
    flagset   = fcntl(pipefd, F_GETFL);
    fcntl(pipefd, F_SETFL, flagset & ~O_NONBLOCK);
    return count;
}

int receiver( int* pipefd, int delay )
{
    const int  bufsize = PIPE_BUF/8; /* Read chunks of size 512 bytes.  */
    const int  usecs = 250000;       /* Used to compute delay.          */
    char   buf[bufsize];             /* Buffer to store reads from pipe */

    close(pipefd[WRITE_END]);        /* Close write-end of pipe.        */
    while ( TRUE ) {
        usleep( usecs*delay +  (rand() % usecs) );  /* Delay a bit.     */
        if ( -1 == read( pipefd[READ_END], buf, bufsize)) /* Read pipe. */
            error_mssge(errno, "read");
    }
    exit(EXIT_SUCCESS);
}

/** A sender process writes messages into the read-end of its pipe
    intermittently, based on the integer delay passed to the function.      */
int sender( int* pipefd, int delay )
{
    message   msg;               /* Message written by sender into pipe */
    close(pipefd[READ_END]);       /* Close read end of pipe.           */
    memset(&(msg.content), 0, 32); /* Clear message content.            */
    sprintf(msg.content, "Hello from process %d.", getpid());
    while ( TRUE ) {
        sleep( delay );   /* Delay a few seconds.                       */
        /* Get the current time, accurate to nanosecond.                */
        if ( -1 == clock_gettime(CLOCK_REALTIME, &(msg.gen_time)) )
            fatal_error(errno, "clock_gettime()");
        write( pipefd[WRITE_END], &msg, sizeof(msg)); /* Write to pipe. */
    }
    exit(EXIT_SUCCESS);
}

void manager(int sndpipe[NSENDERS][2],int* senders, int *recvpipe,int recvpid)
{
    char     line[512];          /* For formatted output */
    char     gentimestr[64];     /* For formatted time of generated message */
    char     str[64];            /* For output */
    char     write_msg[128];     /* For output */
    fd_set   readfds, writefds, readcopy, wrcopy; /* File descriptor sets */
    int      i;
    int      nbytes = 0;
    int      maxfd = 0;
    char     ch = '1';
    message  child_msg;
    int msgsize = sizeof(message);
    struct timespec  ts, diff;

    char fill[PIPE_BUF];
    memset(fill, '0', PIPE_BUF);

    for ( i = 0; i < NSENDERS; i++)
         close(sndpipe[i][WRITE_END]);
    close(recvpipe[READ_END]);

    /* Create descriptor mask for sending children. */
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);                /* Watch standard input. */

    FD_ZERO(&writefds);                /* Watch write end of receiver pipe. */
    FD_SET(recvpipe[WRITE_END], &writefds);
    maxfd = recvpipe[WRITE_END];       /* Make this maxfd for now.          */

    for ( i = 0; i < NSENDERS; i++) { /* Watch read end of each sender pipe.*/
        FD_SET(sndpipe[i][READ_END], &readfds);
        if ( sndpipe[i][READ_END] > maxfd )
            maxfd = sndpipe[i][READ_END];  /* Find max descriptor value.    */
    }
    readcopy = readfds;                  /* Make copies of descriptor sets. */
    wrcopy = writefds;
    fillpipe(recvpipe[WRITE_END]);       /* Fill pipe to capacity.          */

    while (select( maxfd+1, &readcopy, &wrcopy, NULL, NULL ) > 0 ) {
        /* First check pipe to receiving child. */
        if ( FD_ISSET(recvpipe[WRITE_END], &wrcopy) ) {
            if ( -1 == (nbytes = write( recvpipe[WRITE_END], fill, PIPE_BUF)))
                error_mssge(-1, "Could not write to receiver pipe\n");
            sprintf(write_msg,
                 "Successful write of %d bytes to receiver pipe.\n\n", nbytes);
            write(STDOUT_FILENO, write_msg, strlen(write_msg));
        }

        /* Next check standard input. */
        if ( FD_ISSET(STDIN_FILENO, &readcopy) )
            if ( read(STDIN_FILENO, &ch, 1) > 0 && ( ch == 'q') )
                break;  /* User wants to quit, so break loop. */

        /* Now check the pipe from each sending child.  */
        for ( i = 0; i < NSENDERS; i++ ) {
            if ( FD_ISSET(sndpipe[i][READ_END], &readcopy)) {
                /* it is ready to read */
                memset(&child_msg, 0, msgsize);
                if ( read( sndpipe[i][READ_END], &child_msg, msgsize) > 0 ) {
                    if ( -1 == clock_gettime(CLOCK_REALTIME, &ts) )
                        fatal_error(errno, "clock_gettime()");
                    memset(&diff, 0, sizeof(diff));
                    timespec_diff(ts, child_msg.gen_time, &diff);
                    print_time(diff, 1, str);
                    print_time(child_msg.gen_time, 0, gentimestr);
                    sprintf(line,"Message from sender %d (transit-time = %s):\n"
                                 "    Content = \"%s\" Sent at time %s\n\n",
                                 i, str, child_msg.content, gentimestr);
                    write(1, line, strlen(line));
                }
            }
        }
        readcopy = readfds;   /* Restore descriptor sets. */
        wrcopy   = writefds;
    }
    for ( i = 0; i < NSENDERS; i++ ) /* End of loop. Terminate all children.*/
        kill(senders[i], SIGINT);
    kill(recvpid, SIGINT);
    while (waitpid(-1, NULL, WNOHANG) > 0 ) {}  /* Reap their status.       */
    exit(EXIT_SUCCESS);
}

int main( int argc, char* argv[])
{
    int pipefd[NSENDERS][2];  /* Array of pipes for senders   */
    int senderpid[NSENDERS];  /* Sender process PIDs          */
    int recvpid;              /* Receiver process PID         */
    int recvpipefd[2];        /* Receiver process pipe        */

    /* Create a pipe for each child process before fork() .   */
    for ( int i = 0; i < NSENDERS; i++ ) {
        if (pipe(pipefd[i]) == -1 ) /* Create pipe[pipefd[i]. */
            fatal_error(errno, "pipe");
        switch( senderpid[i] = fork() ) {
        case -1 :
            fatal_error(errno, "fork");
        case 0:  /* Child code --- the sender */
            sender(pipefd[i], (i+1) * (1+ (rand()%10)));
        }
    }
    if ( pipe(recvpipefd) == -1 ) /* Create receiver's pipe.  */
        fatal_error(errno, "pipe");
    switch ( recvpid = fork()  ) { /* Fork the receiver.      */
    case -1 :
        fatal_error(errno, "fork");
    case 0:
        receiver(recvpipefd, RECV_DELAY);
    }
    /* Parent continues here. It calls the manager() function. */
    manager(pipefd, senderpid, recvpipefd, recvpid );
    exit(EXIT_SUCCESS);
}


