/*****************************************************************************
  Title       : sigprocmask_demo1.c
  Author      : Stewart Weiss
  Created on  : December 2, 2023
  Description : Gets orig signal mask, modifies blocked set, restores orig
  Purpose     : To show how to create signal masks for blocking signals
  Usage       : sigprocmask_demo1
                 This adds SIGINT to the set of blocked signals, so first do
                 not send a Ctrl-C and watch how it runs. Then send a Ctrl-C
                 before first loop ends and see how the signal is delayed.
                 Then type the Ctrl-C in the second loop.
  Build with  : gcc -I../include -o sigprocmask_demo1 sigprocmask_demo1.c \
                     -L../lib -lspl
******************************************************************************
  Copyright (C) 2023 - Stewart Weiss

  This code is free software; you can use, modify, and redistribute it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; either version 3 of the License, or (at your
  option) any later version. This code is distributed WITHOUT ANY WARRANTY;
  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
  PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.

*****************************************************************************/
#include  "common_hdrs.h"
#include  <signal.h>

void catch_sigint( int signum )
{
    printf("  Caught SIGINT\n"); /* UNSAFE */
}

int main(int argc, char* argv[])
{
    int i;
    sigset_t blocked_set;

    if ( SIG_ERR == signal(SIGINT, catch_sigint) )
        fatal_error(errno, "signal()");
    /* Create a signal set with just SIGINT, and block with it. */
    sigemptyset(&blocked_set);
    sigaddset(&blocked_set, SIGINT);
    if ( -1 == sigprocmask(SIG_BLOCK, &blocked_set, NULL) )
        fatal_error(errno, "sigprocmask()");

    printf("SIGINT is blocked; sleeping for 5 seconds."
           " Try entering a few CTRL-Cs.\n");
    for ( i = 1; i <= 1000; i++ )
        usleep(5000);

    if ( -1 == sigprocmask(SIG_UNBLOCK, &blocked_set, NULL) )
        fatal_error(errno, "sigprocmask()");
    printf("SIGINT is no longer blocked. Enter a few CTRL-Cs.\n");
    for ( i = 1; i <= 5; i++ )
        usleep(800000);

    return 0;
}

