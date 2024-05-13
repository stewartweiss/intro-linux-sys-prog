/*****************************************************************************
  Title          : waitpid_demo.c
  Author         : Stewart Weiss
  Created on     : April 29, 2024
  Description    : Creates 4 child processes and waits for them
  Purpose        : To show how to use a loop to wait for multiple children.
  Usage          : waitpid_demo
  Build with     : gcc -Wall -I../include -L../lib -g -o waitpid_demo \
                          waitpid_demo.c -lspl

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

#define  PRODUCER_EXIT_VAL  63
#define  CONSUMER_EXIT_VAL  127
#define  CONSUME_TIME       30
#define  PRODUCE_TIME       20

void print_status( pid_t pid, int status )
{
    if (WIFEXITED(status))
        printf("\nProcess %d exited with status %d\n", pid,  WEXITSTATUS(status));
    else if ( WIFSIGNALED(status) )
        printf("\nProcess %d terminated by signal %d\n", pid, WTERMSIG(status));
    else if (WIFSTOPPED(status))
       printf("\nProcess %d stopped by signal %d\n", pid, WSTOPSIG(status));
    else if (WIFCONTINUED(status))
       printf("\nProcess %d continued\n", pid);
}


void producer(int fd )
{
    int i;
    char  mychar = 'p';
    printf("Producer is about to start producing data.\n");
    for ( i = 0; i < PRODUCE_TIME; i++ ) {
        usleep(500000);
        if ( -1 == write(fd, &mychar, 1) )
            fatal_error(errno, "write");
    }
    printf("\nProducer (PID=%d) finished producing data; exiting\n", getpid());
    exit(PRODUCER_EXIT_VAL);
}

void consumer(int fd,  int flag)
{
    int i;
    off_t  pos_to_read = 0;
    int nbytes;
    char buffer;

    if (flag > 0) {
        printf("Send a signal to consumer (%d) to terminate it.\n", getpid());
        pause();  /* Wait for signals */
    }
    printf("Consumer receiving the following data from producer:\n");
    for ( i = 0; i < CONSUME_TIME; i++ ) {
        sleep(1);
        if ( -1 == (nbytes = pread(fd, &buffer, 1, pos_to_read) ) )
            fatal_error(errno, "read");
        else if ( nbytes == 0 )
            break;
        else {
            pos_to_read++;
            write(STDOUT_FILENO, &buffer, 1);
        }
    }
    exit(CONSUMER_EXIT_VAL);
}

int main(int argc, char* argv[])
{
    pid_t pid[2];                         /* Store pids of two children.    */
    pid_t retval;                         /* Return value of waitpid()      */
    int fd;                               /* File descriptor of opened file */
    int   status;                         /* Status of waited-on process    */
    int   consumer_off = 0;     /* When set, consumer just waits for signal */

    if ( argc > 1 )
        consumer_off = atoi(argv[1]);

    if (-1 == (fd = open("/tmp/temp", O_CREAT|O_RDWR|O_TRUNC|O_APPEND, 0644)))
        fatal_error(errno, "open");

    switch (pid[0] = fork()) {
    case -1:
        fatal_error(errno, "fork");
    case 0:
        printf("Producer PID = %d\n", getpid());
        producer(fd);
    }
    switch (pid[1] = fork()) {
    case -1:
        fatal_error(errno, "fork");
    case 0:
        printf("Consumer PID = %d\n", getpid());
        consumer(fd, consumer_off);
    }
    int i = 0;
    do {
        if (-1 == (retval = waitpid(pid[i],&status,WUNTRACED|WCONTINUED|WNOHANG))){
            if ( errno != ECHILD )
                fatal_error(errno, "waitpid()");
        }
        else if ( 0 == retval )
            continue;
        else {
            print_status( retval, status );
            if ( WIFEXITED(status) || (WIFSIGNALED(status))  ) {
                if ( i++ == 0 )
                    printf("\nProducer finished; waiting for "
                            "consumer to finish.\n");
            }
        }
        sleep(1);
    } while (i < 2);
    close(fd);
    exit(EXIT_SUCCESS);
}
