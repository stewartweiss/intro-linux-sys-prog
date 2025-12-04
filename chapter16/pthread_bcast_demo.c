/******************************************************************************
  Title          : pthread_bcast_demo.c
  Author         : Stewart Weiss
  Created on     : December  1, 2025
  Description    : Creates some threads that synchronize with a pseudo-barrier
  Purpose        : Shows how to use pthread_cond_wait() as a barrier.
  Usage          : pthread_bcast_demo
  Build with     : gcc -Wall -g -I../include -o pthread_bcast_demo \
                      L../lib  pthread_bcast_demo.c -pthread -lspl -lm

******************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
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
/*
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
*/
/*****************************************************************************
                                Global, Shared Data
*****************************************************************************/

#define   NUM_THREADS         8
#define   NUM_ITERATIONS      4

pthread_mutex_t  mutex;
pthread_cond_t   all_threads_ready    = PTHREAD_COND_INITIALIZER;
pthread_cond_t   all_threads_at_start = PTHREAD_COND_INITIALIZER;

pthread_mutex_t  count_mutex;
int  count = 0;

/*****************************************************************************
                                Thread Function
*****************************************************************************/

void *f( void * data)
{
    printf("Starting up thread %ld\n", (long) data);
    pthread_mutex_lock(&mutex);
    count++;
    if ( count < NUM_THREADS )
        pthread_cond_wait(&all_threads_ready, &mutex);
    /* count reached N, so all threads proceed. */
    pthread_cond_broadcast(&all_threads_ready);
    printf("Thread %ld passed the first barrier\n", (long) data);

    count--;
    pthread_mutex_unlock(&mutex);
    pthread_mutex_lock(&count_mutex);
    if ( count > 0 )
        pthread_cond_wait(&all_threads_at_start, &count_mutex);
    printf("Thread %ld passed the second barrier\n", (long) data);

    pthread_cond_broadcast(&all_threads_at_start);
    pthread_mutex_unlock(&count_mutex);
    return NULL;
}


/*****************************************************************************
                                   Main Program
*****************************************************************************/

int main(int argc, char* argv[])
{
    long        t;
    pthread_t  threads[NUM_THREADS];
    pthread_mutexattr_t    attr;


    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutex_init(&count_mutex, NULL);

    /* Create the threads that will acquire a mutex and then wait on the
       condition variable. */

    for ( t = 0 ; t < NUM_THREADS; t++)
        pthread_create(&threads[t], NULL, f, (void*) t );

    /* Main thread waits for the others to exit. */
    for ( t = 0 ; t < NUM_THREADS; t++)
        pthread_join(threads[t], (void**) NULL);

    /* Clean-up */
    pthread_mutexattr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&all_threads_ready);
    pthread_cond_destroy(&all_threads_at_start);
    pthread_exit (NULL);
}



