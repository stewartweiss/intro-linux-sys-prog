/*****************************************************************************
  Title          : pthread_detach_demo.c
  Author         : Stewart Weiss
  Created on     : December 17, 2011
  Description    : Create a detached thread that continues after main exits
  Purpose        : To demonstrate how to create detached thread and why
  Usage          : thread_detach_demo
  Build with     : gcc -Wall -o pthread_detach_demo -I../include -L../lib \
                   pthread_detach_demo.c  -lspl -lrt -pthread

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
#include <pthread.h>

/* The thread routine. This just prints out the data in the passed in argument
   but very slowly, using usleep() to slow it down.
*/
void *thread_routine(void * arg)
{
    int   i;
    int   bufsize = strlen(arg);
    int   fd = 1;

    printf("Child is running...\n");
    for (i = 0; i < bufsize; i++) {
	       usleep(500000);
	       write(fd, arg+i, 1);
    }
    printf("\nChild is now exiting.\n");
    return(NULL);
}

int main(int argc, char *argv[])
{
    /* Some artificial data */
    char * buf = "abcdefghijklmnopqrstuvwxyz";

    pthread_t thread;
    pthread_attr_t attr;

    /* Set the attribute structure to create a detached thread */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (pthread_create(&thread, NULL, thread_routine, (void *)(buf)))
        fatal_error(errno, "error creating a new thread");

    printf("Main is now exiting.\n");
    pthread_exit(NULL);
}

