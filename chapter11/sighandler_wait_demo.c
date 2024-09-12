/*****************************************************************************
  Title          : sighandler_wait_demo1.c
  Author         : Stewart Weiss
  Created on     : April 29, 2024
  Description    : Creates 4 child processes and waits for them in handler
  Purpose        : To show how to use a SIGCHLD handler to wait for children
  Usage          : sighandler_wait_demo1 [num num num num]
                   If supplied, the numbers are used to control whether
                   child processes pause or not. See below.
  Build with     : gcc -Wall -I../include -L../lib -g \
                  -o sighandler_wait_demo1 sighandler_wait_demo1.c -lspl

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
static volatile int sigchld_count = 0;

void print_status( pid_t pid, int status )
{
    if (WIFEXITED(status))
        printf("Process %d exited with status %d\n", pid,  WEXITSTATUS(status));
    else if ( WIFSIGNALED(status) ) {
        printf("Process %d terminated by signal %d", pid, WTERMSIG(status));
    #ifdef WCOREDUMP
        if ( WCOREDUMP(status) )
            printf( " and a core dump took place");
        printf(".\n");
    #endif
    }
    else if (WIFSTOPPED(status))
        printf("Process %d stopped by signal %d\n", pid, WSTOPSIG(status));
    else if (WIFCONTINUED(status)) {
        printf("Process %d continued\n", pid);
    }
}

void handle_sigchld(int signum, siginfo_t *siginfo, void *unused)
{
    int status;                 /* To store collected status                */
    sigset_t blocked_set;       /* For blocking SIGCHILD to count signals   */
    pid_t waitedfor;            /* Return value of waitpid()                */
    static char errmssge[] = "waitpid in SIGCHLD handler found no"
                             " more reapable children.\n";

    int saved_errno = errno;    /* Save the errno on entry to the handler.  */
    sigemptyset(&blocked_set);
    sigaddset(&blocked_set, SIGCHLD);  /* Set up signal mask.               */
    waitedfor = waitpid(siginfo->si_pid, &status, WUNTRACED | WCONTINUED);
    if (  waitedfor  < 0 )      /* No child to wait for */
        write(2, errmssge, sizeof errmssge);
    else if ( siginfo->si_code == CLD_EXITED ||
              siginfo->si_code == CLD_KILLED ||
              siginfo->si_code == CLD_DUMPED  ) {

        /* The child terminated or way or another.
           Block SIGCHLD while incrementing a signal counter
           and printing the status because we use printf(). */

        if ( -1 == sigprocmask(SIG_BLOCK, &blocked_set, NULL) )
            error_mssge(errno, "sigprocmask()");
        sigchld_count++;
        printf("Handler reaping:");
        print_status(siginfo->si_pid, status);
        /* Unblock SIGCHLD now. */
        if ( -1 == sigprocmask(SIG_UNBLOCK, &blocked_set, NULL) )
            error_mssge(errno, "sigprocmask()");
        }
    else { /* A stop or continue signal */
        if ( -1 == sigprocmask(SIG_BLOCK, &blocked_set, NULL) )
            error_mssge(errno, "sigprocmask()");
        print_status(siginfo->si_pid, status);
        if ( -1 == sigprocmask(SIG_UNBLOCK, &blocked_set, NULL) )
            error_mssge(errno, "sigprocmask()");
    }
    errno = saved_errno;
}

void child( int exit_val)
{
    if ( exit_val == 0 )
        pause();
    else
        sleep(exit_val);
    exit(exit_val);
}


int main(int argc, char* argv[])
{
    pid_t pid[NUM_CHILDREN], w;
    int   exitval[NUM_CHILDREN] = {0,0,0,0}; /* Default values for children */
    struct sigaction sigact;
    int status, n;
    sigset_t  blocked_set;

    sigemptyset(&blocked_set);
    sigaddset(&blocked_set, SIGCHLD);

    /* Get child process exit codes from command line. */
    if ( argc > 1 ) {
        for ( int i = 0; i < argc-1 && i < NUM_CHILDREN; i++ ) {
            n = atoi(argv[i+1]);
            exitval[i] = n > 0? n : 0;
        }
    }
    /* Establish SIGCHLD handler. */
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags  =   SA_SIGINFO;
    sigact.sa_sigaction =  handle_sigchld;
    sigaction(SIGCHLD, &sigact, NULL);

    /* Create the child processes with their exit codes, to run child(). */
    for (int i = 0; i < NUM_CHILDREN; i++) {
        switch (pid[i] = fork()) {
        case -1:
            fatal_error(errno, "fork");
        case 0:
            printf("Child %d PID = %d\n", i, getpid());
            child(exitval[i]);
        default:
            break;
        }
    }

    /* Delay now to allow the sending signals and letting child processes run
       until they exit. The amount of time is easily adjusted. If the
       sigchld_count equals the number of child processes, they all
       terminated, and the loop breaks. Otherwise it continues sleeps.      */
    for ( int i = 0; i < NUM_CHILDREN; i++ ) {
        if (  sigchld_count == NUM_CHILDREN )
             break;
        sleep(6);
    }

    /* Because it's possible that signals were lost, there may be zombies when
       the program is ready to exit. The following loop reaps the zombies and
       prints a message indicating that the main() function did the reaping,
       not the handler.
       This code MUST block SIGCHLD because there is a possibility that some
       child did not yet terminate when we reach here. If so, a SIGCHLD will
       be sent and the handler will run. It is possible for the wait in the
       handler and the wait here to interrupt one another, and for the status
       to be corrupted.  By blocking SIGCHLD, it forces the handler to run
       after the main program reaps the children. */

    if ( -1 == sigprocmask(SIG_BLOCK, &blocked_set, NULL) )
        fatal_error(errno, "sigprocmask()");
    do {
        w = waitpid(-1, &status, WNOHANG ); /* Non-blocking wait */
        if ( -1 == w ) {  /* -1 means no more children need reaping. */
            printf("All child processes are reaped.\n");
        }
        else if ( 0 < w ) { /* It reaped a child. */
            printf("main() reaping:  ");
            print_status(w, status);
        }
    } while ( w >= 0 ); /* Run until all have been reaped. */
    if ( -1 == sigprocmask(SIG_UNBLOCK, &blocked_set, NULL) )
        fatal_error(errno, "sigprocmask()");


    exit(EXIT_SUCCESS);
}
