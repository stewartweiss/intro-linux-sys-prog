/*****************************************************************************
  Title       : signal_demo2.c
  Author      : Stewart Weiss
  Created on  : November 25, 2023
  Description : Sets disposition of SIGINT AND SIGQUIT to SIG_IGN
  Purpose     : To show how to ignore signals.
  Usage       : signal_demo2
                   While it is running, enter ^C and ^\ characters.
  Build wit   : gcc -I../include -o signal_demo2 signal_demo2.c -L../lib -lspl

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

int main(int argc, char* argv[])
{
    int i;
    if ( SIG_ERR == signal( SIGINT,  SIG_IGN ))   /* ignore Ctrl-C */
        fatal_error(errno, "signal()");
    if ( SIG_ERR == signal( SIGQUIT, SIG_IGN ))  /* ignore Ctrl-\ */
        fatal_error(errno, "signal()");
    for( i = 10; i > 0; i-- ) {
        printf("Try to kill me with ^C or ^\\. "
               "Seconds remaining: %2d\n", i);
        sleep(1);
    }
    return 0;
}

