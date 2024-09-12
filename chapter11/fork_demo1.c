/******************************************************************************
  Title          : fork_demo1.c
  Author         : Stewart Weiss
  Created on     : Apr. 25, 2024
  Description    : Simple example of fork() system call
  Purpose        : To demonstrate that child and parent get different
                   return values from fork
  Usage          : fork_demo1
  Build with     : gcc -Wall -I../include -L../lib -g -o forkdemo0 \
                          fork_demo1.c -lspl

******************************************************************************
* Copyright (C) 2024 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include "common_hdrs.h"

int main(int argc, char* argv[])
{
    pid_t  result;

    if ( -1 == ( result = fork() ) )
        fatal_error(errno, "fork");
    else if ( 0 == result )   /* Child executes this branch. */
        printf("I am the child process. My PID is %d\n", getpid());
    else  /* Parent executes this branch */
        printf("I am the parent process. My PID is %d\n", getpid());
    return 0;
}
