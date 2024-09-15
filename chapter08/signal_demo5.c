/*****************************************************************************
  Title       : signal_demo5.c
  Author      : Stewart Weiss
  Created on  : November 25, 2023
  Description : Tries to install universal signal handler for all signals
  Purpose     : To show how to install one handler for all signals
  Usage       : signal_demo5
                   Observe the error messages.
  Build with  : gcc -I../include -o signal_demo5 signal_demo5.c -L../lib -lspl

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
#define _GNU_SOURCE
#include  "common_hdrs.h"
#include  <signal.h>

void all_signal_handler(int signum)
{
    printf("Caught signal SIG%s\n", sigabbrev_np(signum)); /* UNSAFE */
}

int main()
{
    int	i;
    printf("PID=%d\n", getpid());
    for ( i = 1; i < NSIG; i++ )
        if ( SIG_ERR == signal( i,  all_signal_handler ))
            printf("Could not install handler for signal SIG%s, (%d)\n",
                   sigabbrev_np(i), i);
    printf("Use SIGKILL to terminate this program.\n");
    while ( TRUE ) continue;
    return 0;
}
