/*****************************************************************************
  Title       : sigact_demo1.c
  Author      : Stewart Weiss
  Created on  : December 9, 2023
  Description : Displays siginfo information about signal sent to it
  Purpose     : To show how to use siginfo_t info data
  Usage       : sigact_demo1
                   In a second terminal window, send a SIGINT, following
                   instructions printed on terminal by this program.
  Build with  : gcc -o sigact_demo1 -I../include -L../lib sigact_demo1.c -lspl


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

#include "common_hdrs.h"
#include <signal.h>

void sig_handler (int signo, siginfo_t *info,  void *context)

{
    printf ("Signal number: %d\n", info->si_signo);      /* UNSAFE */
    printf ("PID of sender: %d\n", info->si_pid);        /* UNSAFE */
    printf ("UID of sender: %d\n\n", info->si_uid);      /* UNSAFE */
    /* Force the process to terminate by raising SIGTERM,
       for which we have no handler. */
    if ( signo == SIGINT)
        raise(SIGQUIT);
    else
        raise(SIGTERM);
}

int main (int argc, char* argv[])
{
    struct sigaction the_action;

    the_action.sa_flags     = SA_SIGINFO;
    the_action.sa_sigaction = sig_handler;
    sigemptyset(&(the_action.sa_mask));
    if ( -1 == sigaction(SIGINT, &the_action, NULL))
        fatal_error(errno, "sigaction()");
    if ( -1 == sigaction(SIGQUIT, &the_action, NULL))
        fatal_error(errno, "sigaction()");
    printf ("Open a second terminal window and send SIGINT "
            "by entering kill -s SIGINT %d\n", getpid());
    pause();
    return 0;
}






