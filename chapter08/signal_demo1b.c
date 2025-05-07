/*****************************************************************************
  Title          : signal_demo1b.c
  Author         : Stewart Weiss
  Created on     : December 6, 2023
  Description    : Installs signal handlers for ^C and ^\
  Purpose        : To show how to install signal handlers and see their effect
  Usage          : signal_demo1
                   While it is running, enter ^C and ^\ a few times.
  Build with     : gcc -Wall -g -o signal_demo1b -I../include -L../lib \
                          signal_demo1b.c [-DMOUSETRAPMODE] -lspl

   Notes
   This differs from signal_demo1.c because it allows the user to force the
   handler to behave in mouse-trap mode, meaning that it only runs for the 
   first signal, after which default behavior occurs.

*****************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                        *
*                                                                           *
* This code is free software; you can use, modify, and redistribute it      *
* under the terms of the GNU General Public License as published by the     *
* Free Software Foundation; either version 3 of the License, or (at your    *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY; *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A     *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.               *
****************************************************************************/

#include "common_hdrs.h"
#include  <signal.h>

/****************************************************************************/
/*                                Handlers                                  */
/*****************************************************************************/
void catch_sigint(int signum)
{
#ifdef MOUSETRAPMODE
    signal(signum, SIG_DFL);
#endif
    printf("I'm not killed by CTRL-C!\n");
}

void catch_sigquit(int signum)
{
    printf("I'm not killed by CTRL-\\!\n");
}

int main()
{
    if ( SIG_ERR == signal( SIGINT,  catch_sigint) )
        fatal_error(errno, "signal()");
    if ( SIG_ERR == signal( SIGQUIT, catch_sigquit) )
        fatal_error(errno, "signal()");
    for (int i = 20; i > 0; i-- ) {
        printf("Try to terminate me with ^C or ^\\.\n");
        sleep(1);
    }
    return 0;
}
