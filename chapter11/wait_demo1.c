/*****************************************************************************
  Title          : wait_demo1.c
  Author         : Stewart Weiss
  Created on     : April 29, 2024
  Description    : Creates 4 child processes and waits for them
  Purpose        : To show how to use a loop to wait for multiple children.
  Usage          : wait_demo1
  Build with     : gcc -Wall -I../include -L../lib -g -o wait_demo1 \
                          wait_demo1.c -lspl

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
    int   i, num_children = 4;

    for (i = 0; i < num_children; i++) {
        switch ( fork()) {
        case -1:
            fatal_error(errno, "fork");
        case 0:
            sleep(2);                    /* Children sleep and then exit.   */
            exit(i);
        default:
            break;                       /* Parent just waits for children. */
        }
    }
    while ((pid = wait(NULL)) != -1)
        printf("Child %d terminated.\n", pid);
    if ( ECHILD != errno )
        fatal_error(errno, "wait");
    exit(EXIT_SUCCESS);
}
