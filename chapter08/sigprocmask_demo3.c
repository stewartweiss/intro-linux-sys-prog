/*****************************************************************************
  Title       : sigprocmask_demo3.c
  Author      : Stewart Weiss
  Created on  : December 2, 2023
  Description : Counts and prints SIGINTs delivered
  Purpose     : To show how to share data between handler and rest of program
  Usage       : sigprocmask_demo3
                Enter CTRL-Cs and observe that they're counted correctly.
  Build with  : gcc -I../include -o sigprocmask_demo3 sigprocmask_demo3.c \
                     -L../lib -lspl

******************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include  "common_hdrs.h"
#include  <signal.h>

static volatile sig_atomic_t sig_received = 0;

void catch_sigint(int signum)
{
    sig_received = 1;
}

int main (int argc, char* argv[])
{
    sigset_t blockedset;
    int count = 0;

    /* Initialize the signal mask and install the handler. */
    sigemptyset (&blockedset);
    sigaddset (&blockedset, SIGINT);
    if ( SIG_ERR == signal(SIGINT, catch_sigint) )
        fatal_error(errno, "signal()");
    printf("PID=%d\n Enter CTRL-\\ to end this program.\n", getpid());
    while (TRUE)    {
        /* Block the signal while we print the count. */
        if ( -1 == sigprocmask (SIG_BLOCK, &blockedset, NULL) )
            fatal_error(errno, "sigprocmask()");
        if ( sig_received ) {
           count++;
           sig_received = 0;
        }
        printf("\n%d SIGINTs received so far\n", count);
        /* Unblock the signal, allowing handler to run. */
        if ( -1 == sigprocmask (SIG_UNBLOCK, &blockedset, NULL) )
            fatal_error(errno, "sigprocmask()");
        pause();
    }
}
