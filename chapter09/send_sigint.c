/******************************************************************************
  Title          : signal_demo1.c
  Author         : Stewart Weiss
  Created on     : March 22, 2008
  Description    : Installs signal handlers for ^C and ^\
  Purpose        : To show how to install signals and the effect of the handlers
  Usage          : signal_demo1
                   While it is running, enter ^C and ^\ a few times.
  Build with     : gcc -Wall -g -o signal_demo1
  Modifications  :

 ******************************************************************************
 * Copyright (C) 2019 - Stewart Weiss
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.


*****************************************************************************/

#include  "common_hdrs.h"
#include  <signal.h>

int main(int argc, char* argv[])
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
