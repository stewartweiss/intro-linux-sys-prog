/*****************************************************************************
  Title          : wait_demo2.c
  Author         : Stewart Weiss
  Created on     : April 29, 2024
  Description    : Creates a child process and prints its termination status
  Purpose        : To show how to use the macros to test the status in wait()
  Usage          : wait_demo2 [exit-status]
  Build with     : gcc -Wall -I../include -L../lib -g -o wait_demo2 \
                          wait_demo2.c -lspl
  Note:
        If the program is run without a command-line argument, you need to
        send a signal to terminate the child. Otherwise the child exits
        passing the command-line argument as its value.

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
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    pid_t pid;
    int   status;

    switch (fork()) {
    case -1:
        fatal_error(errno, "fork");
    case 0:
        if ( argc == 1 ) {
            printf("Child PID = %d\n",  getpid());
            pause();    /* Wait for signal. */
        }
        exit(atoi(argv[1]));
    default:
        if ((pid = wait(&status)) != -1) {
            printf("Child %d terminated ", pid);
            if (WIFEXITED(status))
                printf("with exit status %d.\n", WEXITSTATUS(status));
            else if ( WIFSIGNALED(status) ) {
                printf("as a result of signal %d", WTERMSIG(status));
            #ifdef WCOREDUMP
                if ( WCOREDUMP(status) )
                    printf(" and a core dump took place");
            #endif
                printf(".\n");
            }
        }
        else if ( ECHILD != errno )
            fatal_error(errno, "wait");
        exit(EXIT_SUCCESS);
    }
}
