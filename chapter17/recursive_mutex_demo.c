/******************************************************************************
  Title          : recursive_mutex_demo.c
  Author         : Stewart Weiss
  Created on     : December 18, 2011
  Description    :
  Purpose        :
  Usage          : recursive_mutex_demo
  Build with     : gcc -o recursive_mutex_demo recursive_mutex_demo.c -pthread
  Modifications  :

  Notes:


******************************************************************************/
/*
 * Copyright (C) 2019 - Stewart Weiss
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
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
        if ( 0 != pthread_create(&threads[t], NULL, thread_routine, (void *) t))
            fatal_error( errno, "pthread_create");

    for ( int t = 0 ; t < 2; t++) {
        pthread_join(threads[t], (void**) NULL);
    }
    return 0;
}
