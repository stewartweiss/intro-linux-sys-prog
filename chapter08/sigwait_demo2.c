/*****************************************************************************
  Title          : sigwait_demo2.c
  Author         : Stewart Weiss
  Created on     : April 20, 2024
  Description    : Synchronously responds to a few signals
  Purpose        : To demonstrate how sigwait() works
  Usage          : sigwait_demo2
                   Send a few SIGINTs and SIGQUITs.
  Build with     : gcc -Wall -g -I../include -L../lib -o sigwait_demo2 /
                     sigwait_demo2.c -lspl

  Note: After 5 SIGINTS it terminates. This shows how to count them.


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
#include "common_hdrs.h"
#include <signal.h>

volatile sig_atomic_t  sig_received = 0;

void  handler(int signum)
{
    printf("Doing useful work now.\n");
    sig_received = 1;
    raise(SIGTERM);
}


int main(int argc, char *argv[])
{
    struct sigaction  sigact;
    sigset_t mask;
    int sig, count = 0;

    /* Set up signal mask for sigprocmask and sigwait.*/
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGUSR1);
    if ( -1 == sigprocmask(SIG_BLOCK, &mask, NULL) )
        fatal_error(errno, "sigprocmask");


    sigact.sa_handler = handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags  = 0;
    sigaction(SIGINT, &sigact, NULL);
    sigaction(SIGQUIT, &sigact, NULL);
    printf("PID=%d\n", getpid());

    while (TRUE) {
        if ( sigwait(&mask, &sig) != 0 )
             fatal_error(errno, "sigwait");
        printf("Signal %d received\n", sig);
        if ( sig == 2 ) {
            if ( ++count == 5 ) {
                sigdelset(&mask, SIGINT);
                raise(SIGINT);
            break;
            }
        }
    }
    return 0;
}
