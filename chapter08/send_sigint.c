/*****************************************************************************
  Title          : send_sigint.c
  Author         : Stewart Weiss
  Created on     : October 22, 2024
  Description    : Repeatedly sends SIGINT to designated process
  Purpose        : Useful for working with signals
  Usage          : send_sigint <pid>
  Build with     : gcc -I../include -o send_sigint send_sigint.c -L../lib \
                     -lspl

 *****************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
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

int main(int argc, char *argv[])
{
    int	res, pid, i;
    char message[128];
    if ( argc < 2 )
        usage_error("kill_demo <PID of a process to signal>");

    if ( VALID_NUMBER != (res = get_int(argv[1], NO_TRAILING, &pid, message)))
        fatal_error(res, message);

    for ( i = 1; i < 100 ; i++ ) {
        usleep(100000);
        if ( -1 == kill(pid, SIGINT))
            fatal_error(errno, "kill() sending SIGINT");
    }
    printf("Sent %d SIGINT signals to %d\n", i, pid);

    return 0;
}
