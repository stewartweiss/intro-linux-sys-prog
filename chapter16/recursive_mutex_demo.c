/*****************************************************************************
  Title          : recursive_mutex_demo.c
  Author         : Stewart Weiss
  Created on     : December 18, 2011
  Description    : Creates two threads with recursive thread functions
  Purpose        : To demonstrate the use of recursive mutexes
  Usage          : recursive_mutex_demo
  Build with     : gcc -Wall -g -I../include -o recursive_mutex_demo \
                      L../lib  recursive_mutex_demo.c -pthread -lspl


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
#define BOUND    4
pthread_mutex_t        mutex;
struct timespec sleeptime= {0, 250000000};
int count = 0;

void up(long int tid)
{
    pthread_mutex_lock(&mutex);
    printf("Thread %ld acquired lock in up();  count = %d\n",  tid, count);
    nanosleep(&sleeptime, NULL);
    if ( ++count < BOUND )
        up(tid);
    else {
        count = 0;
        printf("Thread %ld returning from up();  count = %d\n",  tid, count);
    }
    pthread_mutex_unlock(&mutex);
}


void * thread_routine( void * data )
{
    up((long) data);
    pthread_exit(NULL);
}

int main( int argc, char *argv[])
{
    pthread_t  threads[2];
    pthread_mutexattr_t    attr;

    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
    for ( int t = 0 ; t < 2; t++)
        if ( 0 != pthread_create(&threads[t], NULL, thread_routine,
            (void *) t))
            fatal_error( errno, "pthread_create");

    for ( int t = 0 ; t < 2; t++) {
        pthread_join(threads[t], (void**) NULL);
    }
    return 0;
}
