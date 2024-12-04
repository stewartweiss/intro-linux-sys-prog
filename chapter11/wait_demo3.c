/*****************************************************************************
  Title          : wait_demo3.c
  Author         : Stewart Weiss
  Created on     : April 29, 2024
  Description    : Creates 4 child processes and waits for them
  Purpose        : To show how to use a loop to wait for multiple children.
  Usage          : wait_demo3
  Build with     : gcc -Wall -I../include -L../lib -g -o wait_demo3 \
                          wait_demo3.c -lspl

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

void  handler(int signum)
{
    printf("Received SIGTERM; cleaning up...\n");
    signal(SIGTERM, SIG_DFL);
    raise(SIGTERM);
}


void child( int exit_val)
{
    struct sigaction  sigact;
    sigset_t mask;
    int sig;
    BOOL  done = FALSE;

    sigact.sa_handler = handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags  = 0;
    sigaction(SIGTERM, &sigact, NULL);
    //printf("PID=%d\n", getpid());

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGHUP);
    if ( -1 == sigprocmask(SIG_BLOCK, &mask, NULL) )
        fatal_error(errno, "sigprocmask");

    while (!done) {
        if ( sigwait(&mask, &sig) != 0 )
             fatal_error(errno, "sigwait");
        switch ( sig ) {
        case SIGINT:
            printf("Child %d received SIGINT\n", getpid());
            done = TRUE;
            break;
        case SIGHUP:
            printf("SIGHUP received\n");
            raise(SIGTERM);
            break;
        default:
             printf("Received some other signal.\n");
        }
    }
    fflush(stdout);
    exit(exit_val);
}


int main(int argc, char *argv[])
{
    pid_t pid[4], w;
    int   i, num_children = 4;
    int   status;

    for (i = 0; i < num_children; i++) {
        switch (pid[i] = fork()) {
        case -1:
            fatal_error(errno, "fork");
        case 0:
            printf("Child %d has PID %d\n", i, getpid());
            child(i+32);
        default:
            break;
        }
    }
    for ( i = 0; i < num_children; i++ ) {
         w = waitpid(pid[i], &status, WUNTRACED | WCONTINUED  );
         if (w == -1)
             fatal_error(errno, "waitpid");

         if (WIFEXITED(status))
             printf("Process %d exited with status %d\n", w,  WEXITSTATUS(status));
         else if (WIFSTOPPED(status))
             printf("Process %d stopped by signal %d\n", w, WSTOPSIG(status));
         else if (WIFCONTINUED(status))
             printf("Process %d continued\n", w);
         else if ( WIFSIGNALED(status) ) {
            printf("Process exited as a result of signal %d ", WTERMSIG(status));
            if ( WCOREDUMP(status) )
                printf("and a core dump took place");
            printf(".\n");
        }
    }
    if ( ECHILD != errno )
        fatal_error(errno, "wait");
    exit(EXIT_SUCCESS);
}
