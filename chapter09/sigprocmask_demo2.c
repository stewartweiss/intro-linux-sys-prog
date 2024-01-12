/*****************************************************************************
  Title       : sigprocmask_demo2.c
  Author      : Stewart Weiss
  Created on  : December 2, 2023
  Description : Blocks all possible signals (except SIGKILL and SIGSTOP)
  Purpose     : To show how blocking works
  Usage       : sigprocmask_demo2
                Open a second window, and use kill to send different
                signals to this process.
  Build with  : gcc -I../include -o sigprocmask_demo2 sigprocmask_demo2.c \
                     -L../lib -lspl

 *****************************************************************************
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

int main(int argc, char* argv[])
{
    sigset_t signals, prevsignals;

    printf("PID=%d\n",getpid());
    sigfillset(&signals);
    if ( -1 == sigprocmask(SIG_BLOCK, &signals, &prevsignals) )
        fatal_error(errno, "sigprocmask()");

    while ( TRUE ) {
        printf("Try sending signals to me. "
               "Use SIGKILL to terminate me, SIGSTOP to stop me.\n");
        sleep(5);
    }
    if ( -1 == sigprocmask(SIG_SETMASK, &prevsignals, NULL) )
        fatal_error(errno, "sigprocmask()");
    return 0;
}

