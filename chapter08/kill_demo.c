/*****************************************************************************
  Title       : kill_demo.c
  Author      : Stewart Weiss
  Created on  : November 25, 2023
  Description : Sends a SIGINT then a SIGTERM to specified process
  Purpose     : A tool to help show how signal handlers work
  Usage       : Start up signal_demo3, capture its PID, and run
                        kill_demo <pid>
  Build with  : gcc -I../include -o kill_demo kill_demo.c -L../lib -lspl

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
    int	res, pid;
    char message[128];
    if ( argc < 2 )
        usage_error("kill_demo1 <PID of a process to signal>");

    if ( VALID_NUMBER != (res = get_int(argv[1], NO_TRAILING, &pid, message)))
        fatal_error(res, message);

    printf("%s sending SIGINT to  process %d.\n", argv[0], pid);
    if ( -1 == kill(pid, SIGINT))
        fatal_error(errno, "kill() sending SIGINT");
    sleep(1); /* Give a chance for signal to be sent. */
    printf("%s sending SIGTERM to terminate process %d.\n", argv[0], pid);
    if ( -1 == kill(pid, SIGTERM))
         fatal_error(errno, "kill() sending SIGTERM");

    return 0;
}
