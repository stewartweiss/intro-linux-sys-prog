/*****************************************************************************
  Title          : fork_demo4.c
  Author         : Stewart Weiss
  Created on     : April 13, 2024
  Description    : Forks several processes with potential problem
  Purpose        : To demonstrate that I/O buffering alters expected behavior
  Usage          : fork_demo4
  Build with     : gcc -Wall -g -o forkdemo2 forkdemo2.c
  Notes:
  First run this program without redirecting output. Run it again, this
  time piping the output to sort. What is the difference?

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

int main( int argc, char* argv[] )
{
    pid_t newpid;
    int   num_children = 4;

    printf("About to create many processes...\n");
    while (  num_children-- > 0 ) {
        newpid = fork();   /* Create a child process. */
        if ( 0 == newpid ) /* If this is a new process, don't create others.*/
            break;
    }
    /* Each process calls getpid() and prints it process ID. */
    printf("Process ID = %d\n", getpid());
    return 0;
}
