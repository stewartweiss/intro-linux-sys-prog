/*****************************************************************************
  Title          : sigwait_demo1.c
  Author         : Stewart Weiss
  Created on     : April 20, 2024
  Description    : Synchronously responds to a few signals
  Purpose        : To demonstrate how sigwait() works
  Usage          : sigwait_demo1
  Build with     : gcc -Wall -g -I../include -L../lib -o sigwait_demo1 /
                     sigwait_demo1.c -lspl

******************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
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

void  quithandler(int signum)
{
    printf("Exiting because of SIGQUIT\n");
    signal(SIGQUIT, SIG_DFL);
    raise(SIGQUIT);
}

int main(int argc, char *argv[])
{
    struct sigaction  sigact;
    sigset_t mask;
    int sig;

    /* Install the handler for SIGQUIT. */
    sigact.sa_handler = quithandler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags  = 0;
    sigaction(SIGQUIT, &sigact, NULL);

    /* Set up signal mask for sigprocmask and sigwait.*/
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGUSR1);
    if ( -1 == sigprocmask(SIG_BLOCK, &mask, NULL) )
        fatal_error(errno, "sigprocmask");

    printf("PID=%d\n", getpid()); /* Print the PID so that we can kill it. */

    while (TRUE) {
        if ( sigwait(&mask, &sig) != 0 )
             fatal_error(errno, "sigwait");
        switch ( sig ) {
        case SIGINT: printf("SIGINT received\n"); break;
        case SIGHUP: printf("SIGHUP received\n"); break;
        default:
             printf("Received some other signal.\n");
        }
    }
    return 0;
}
