/*****************************************************************************
  Title          : pthread_create_demo.c
  Author         : Stewart Weiss
  Created on     : June 16, 2024
  Description    : Creates a single thread that prints Hello, World
  Purpose        : To show how to use pthread_create()
  Usage          : pthread_create_demo
  Build with     : gcc -Wall -o pthread_create_demo -I../include -L../lib \
                   pthread_create_demo.c  -lspl -lm -lrt -pthread


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

/* Thread's start function */
void* hello_world( void * world)
{
    long  exit_value = 0; /* Exit value must be the same size as a pointer. */
    printf("Hello world from  %s!\n", (char*) world);
    pthread_exit((void*) exit_value);
}


int main( int argc, char *argv[])
{
    pthread_t  newthread;  /* Stores thread ID. */
    long      *status; /* To store thread exit status, this  must be long. */

    if ( 0 != pthread_create(&newthread, NULL, hello_world,"Pluto") )
        fatal_error(errno, "pthread_create");

    /* Now wait for the child thread */
    if ( 0 != pthread_join(newthread, (void**) (&status)) )
        fatal_error(errno, "pthread_join");
    printf("Child exited with status %ld\n", (long) status);
    return 0;
}
