/****************************************************************************
  Title          : fork_demo2.c
  Author         : Stewart Weiss
  Created on     : Apr. 25, 2011
  Description    : Simple example of fork() system call
  Purpose        : To demonstrate that the new process gets copies of the
                   entire memory image.

  Usage          : fork_demo2
  Build with     : gcc -Wall -g -o fork_demo2 fork_demo2.c

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

const char str[] = "On the heap.";
int   globalvar = 10;

int main(int argc, char* argv[])
{
    int    localvar = 0;
    pid_t  result, mypid;
    char*   heapvar;

    if ( NULL == ( heapvar = calloc(sizeof(str)+1, 1 ) ) )
        fatal_error(errno, "calloc");

    memcpy(heapvar, str, strlen(str));
    heapvar[strlen(str)] = '\0';

    printf("This is printed by the parent process before the call"
           " to fork():\npid = %d, localvar = %d, globalvar = %d, "
           " heapvar = \"%s\" \n\n", getpid(), localvar, globalvar, heapvar);

    if ( -1 == ( result = fork() ) ) {
        fatal_error(errno, "fork");
    }
    else if ( 0 == result ) {  /* Child executes this branch. */
        mypid = getpid();
        printf("This is printed by the  child process:\n");
        printf("Child PID = %d, localvar = %d, globalvar = %d, "
               "heapvar = \"%s\"\n", mypid, localvar, globalvar, heapvar);
        localvar = 1;        /* Make changes to these variables. */
        globalvar = mypid;
        memset(heapvar, 'x', strlen(heapvar));
        printf(" Child process will now assign new values to these variables.\n\n");
    }
    else { /* Parent executes this branch */
        mypid = getpid();
        sleep(2);  /* Sleep long enough for child's output to appear first. */
    }
    /* Both processes continue here. */
    if ( 0 == result )
        printf("Child printing:");
    else
        printf("Parent printing:");

    printf("\nMy pid is %d. The variables "
           "have the following values in my address space:\n "
           "localvar = %d, globalvar = %d, heapvar = \"%s\" \n\n",
            mypid, localvar, globalvar, heapvar);
return 0;
}
