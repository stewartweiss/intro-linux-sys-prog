/*****************************************************************************
  Title          : sighandler_wait_demo2.c
  Author         : Stewart Weiss
  Created on     : April 29, 2024
  Description    : A signal-safe version of sighandler_wait_demo.c
  Purpose        : To show how to use a loop to wait for multiple children.
  Usage          : sighandler_wait_demo2
  Build with     : gcc -Wall -I../include -L../lib -g -o \
                        sighandler_wait_demo2 sighandler_wait_demo2.c -lspl

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

#define NUM_CHILDREN  4

typedef struct proc_state {
    pid_t         pid;
    sig_atomic_t  index;
} child_state;

child_state  childstat[NUM_CHILDREN];

volatile sig_atomic_t  cstatus[NUM_CHILDREN] = { 0,0,0,0};


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

void print_status2( pid_t pid, int status )
{
    char pidstr[8];
    char statstr1[4];
    char statstr2[4];
    char newline[] = "\n";
    char procword[] = "Process ";
    int  len = sizeof procword;

    lltostr(pid, pidstr,8);

    if (WIFEXITED(status)) {
        lltostr(WEXITSTATUS(status), statstr1, 4);
        char msg2[] = " exited with status";
        write(1, procword, len );
        write(1, pidstr, sizeof pidstr);
        write(1, msg2, sizeof msg2);
        write(1, statstr1, sizeof statstr1);
        write(1, newline, 1);
    }
    else if ( WIFSIGNALED(status) ) {
        lltostr(WTERMSIG(status), statstr2, 4);
        char msg2[] = " terminated by signal";
        write(1, procword, len );
        write(1, pidstr, sizeof pidstr);
        write(1, msg2, sizeof msg2);
        write(1, statstr2, sizeof statstr2);

    #ifdef WCOREDUMP
        if ( WCOREDUMP(status) ) {
            char msg3[] = " and a core dump took place";
            write(1, msg3, sizeof msg3);
        }
    #endif
        write(1, newline, 1);
    }
    else if (WIFSTOPPED(status)) {
        lltostr(WSTOPSIG(status), statstr1, 4);
        char msg2[] = " stopped by signal";
        write(1, procword, len );
        write(1, pidstr, sizeof pidstr);
        write(1, msg2, sizeof msg2);
        write(1, statstr1, sizeof statstr1);
        write(1, newline, 1);
    }
    else if (WIFCONTINUED(status)) {
        char msg2[] = " continued\n";
        write(1, procword, len );
        write(1, pidstr, sizeof pidstr);
        write(1, msg2, sizeof msg2);
    }
}


void handle_sigchld(int signum, siginfo_t *siginfo, void *unused)
{
    int status;                 /* To store collected status                */
    pid_t waitedfor;            /* Return value of waitpid()                */
    static char error_message[] = "waitpid() failed\n"; /* In case of error.*/
    int saved_errno = errno;    /* Save the errno on entry to the handler.  */


    waitedfor = waitpid(siginfo->si_pid, &status, WUNTRACED | WCONTINUED);
    if ( -1 == waitedfor )
        write(2, error_message, sizeof error_message); /* Error             */
    else if ( siginfo->si_code == CLD_EXITED ||
              siginfo->si_code == CLD_KILLED ||
              siginfo->si_code == CLD_DUMPED  ) {
        print_status(waitedfor, status);
    }
    errno = saved_errno;
}


void child( int exit_val)
{
    //sleep(10 + exit_val*5);
    if ( exit_val == -1 )
        pause();
    else
        usleep(exit_val*2000000);
    exit(exit_val);
}


int main(int argc, char *argv[])
{
    pid_t pid[NUM_CHILDREN];
    int   i;
    struct sigaction sigact;
    int done = 0;

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags  =  SA_NODEFER | SA_SIGINFO;
    sigact.sa_sigaction =  handle_sigchld;
    sigaction(SIGCHLD, &sigact, NULL);

    for (i = 0; i < NUM_CHILDREN; i++) {
        switch (pid[i] = fork()) {
        case -1:
            fatal_error(errno, "fork");
        case 0:
            printf("Child %d PID = %d\n", i, getpid());
            sleep(1);
            child(1+2);
        default:
            break;
        }
    }
    sleep(5);
    while ( done != 1 ) {
        sleep(1);
    }
    exit(EXIT_SUCCESS);
}
