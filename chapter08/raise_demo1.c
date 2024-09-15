/*****************************************************************************
  Title       : raise_demo.c
  Author      : Stewart Weiss
  Created on  : November 28, 2023
  Description : Handles SIGTERM by "cleaning up" and then resuming
  Purpose     : To show one use of raise() in signal handlers
  Usage       : raise_demo
                   While it's running send CTRL-\ and CTRL-C to quit.
  Build with  : gcc -I../include -o raise_demo raise_demo.c -L../lib -lspl

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

/****************************************************************************/
/*                                Handlers                                  */
/*****************************************************************************/

void sigtstp_handler(int signum)
{
    if ( SIG_ERR == signal( SIGTSTP, SIG_DFL ) )
        fatal_error(errno, "signal()");
    printf("\ncleaning up in progress...\ndone\n");
    raise( SIGTSTP );
    printf("raise() called to stop process.\n");
}

void sigcont_handler(int signum)
{
    /* When the process is resumed, reset the sigtstp handler so that it
       cleans up again before stopping. */
    if ( SIG_ERR == signal(SIGTSTP, sigtstp_handler) )
        fatal_error(errno, "signal()");
}

int main()
{
    int	i;
    if ( SIG_ERR == signal( SIGTSTP, sigtstp_handler ) )
        fatal_error(errno, "signal()");
    if ( SIG_ERR == signal( SIGCONT, sigcont_handler ) )
        fatal_error(errno, "signal()");
    for( i = 20; i > 0; i-- ) {
        printf("Enter CTRL-Z to stop the process, or CTRL-C to end it.\n");
        sleep(4);
    }
    return 0;
}



