/*****************************************************************************
  Title          : fork_demo5.c
  Author         : Stewart Weiss
  Created on     : April 13, 2024
  Description    : Modification of fork_demo4.c that prevents its races
  Purpose        : To introduce isatty() and O_APPEND
  Usage          : fork_demo5
  Build with     : gcc -Wall -I../include -L../lib -g -o forkdemo5 \
                          fork_demo5.c -lspl


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

int main( int argc, char *argv[] )
{
    pid_t newpid;
    int   num_children = 4;
    int    flags;

    /* Put standard output into atomic append mode */
    flags = fcntl(1, F_GETFL);
    flags |= (O_APPEND);
    if (fcntl( STDOUT_FILENO, F_SETFL,flags) == -1 )
        exit(1);

    printf("About to create many processes...\n");
    if ( !isatty(fileno(stdout)) )
        fflush(stdout);
    while (  num_children-- > 0 ) {
        newpid = fork();   /* Create a child process. */
        if ( newpid == 0 ) /* If this is a new process, don't create others.*/
            break;
    }
    /* Each process calls getpid() and prints it process ID. */
    printf("Process ID = %d\n", getpid());
    return 0;
}
