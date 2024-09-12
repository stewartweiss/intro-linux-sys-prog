/*****************************************************************************
  Title          : pthread_setstacksize.c
  Author         : Stewart Weiss
  Created on     : December 18, 2023
  Description    : Lets user change thread stack size and thread displays it
  Purpose        : To show how to change the thread's stack size and how to
                   display the actual size, which requires a GNU extension to
                   POSIX threads.
  Usage          : pthread_setstacksize stacksize
  Build with     : gcc -L../lib -I../include -o pthread_setstacksize \
                   pthread_setstacksize.c -lspl -pthread

Notes
  The stack size of a thread is part of the pthread_attr_t structure. The
  stack size can be changed by calling pthread_attr_setstacksize() or
  pthread_attr_setstack(). The latter changes not just the size but the stack
  address as well. We use the former here.

  This program begins by getting the default stack size and printing it. It
  then sets the new stack size to be what was specified on the command line.
  No error checking of this value is performed.  The thread is created and
  runs. Within the thread, it gets its actual stack size. There is no Pthreads
  function that a thread can use to get its actual stack size. There are
  functions to get the stack size of an attributes object, but this is not
  necessarily the actual object used by the thread. GNU has an extension that
  lets a thread get its actual attribute structure and this is what we use
  here.

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


#define _GNU_SOURCE     /* To get pthread_getattr_np() declaration */
#include "common_hdrs.h"
#include <pthread.h>


void *thread_routine(void *arg)
{
    size_t         stack_size;
    pthread_attr_t* attr;
    int            retval;

    attr = (pthread_attr_t*) arg;

    /* Get the actual stack size from attr. */
    retval = pthread_attr_getstacksize(attr, &stack_size);
    if ( retval )
        fatal_error(retval, "pthread_attr_getstacksize");

    printf("Actual stack size is \t%zu \t(0x%zx)\n",  stack_size, stack_size);
    pthread_exit( 0 );
}


int main(int argc, char *argv[])
{
    pthread_t       thr;
    pthread_attr_t  attr;
    int             retval;
    size_t          new_stack_size;
    size_t          stack_size;
    char            str[PATH_MAX];

    if ( argc < 2 ) {
        sprintf(str, "usage: %s stacksize\n", argv[0] );
        usage_error(str);
    }

    /* Get requested stack size but don't error-check. */
    new_stack_size = strtoul(argv[1], NULL, 0);

    if ( (retval = pthread_attr_init(&attr)) != 0)
        fatal_error(retval, "pthread_attr_init");

    /* Make it joinable */
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_attr_getstacksize (&attr, &stack_size);
    printf("Default stack size is  \t%zu \t(0x%zx)\n", stack_size, stack_size);
    printf("New stack size will be \t%zu \t(0x%zx)\n",
            new_stack_size, new_stack_size );

    /* Set the stack size to the one passed to the program */
    if ( (retval = pthread_attr_setstacksize(&attr, new_stack_size)) != 0 )
        fatal_error(retval, "pthread_attr_setstacksize");

    /* Create the thread */
    if ( (retval = pthread_create(&thr, &attr, &thread_routine, &attr)) != 0)
        fatal_error(retval, "pthread_create");

    /* Join the thread before exiting */
    pthread_join(thr, NULL);
    return(0);
}
