/*****************************************************************************
  Title          : pthread_prodcons.c
  Author         : Stewart Weiss
  Created on     : July 1, 2024
  Description    : A multithreaded multiple producer/consumer program
  Purpose        : To demonstrate condition variables and mutexes
  Usage          : pthread_prodcons [-c <num>] [-p <num>] [-m <num>]
                   where -c sets the number of consumers, -p, the number of
                   producers, and -m, the number of data items that producers
                   will produce
  Build with     : gcc -Wall -g -I../include -o pthread_prodcons \
                      L../lib  pthread_prodcons.c -pthread -lspl -lm

  Notes: This is a multi-threaded producer/comsumer program that outputs
        informatyive messages

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

/*----------------------------------------------------------------------------
                                Global, Shared Data
----------------------------------------------------------------------------*/

#define   MAX_ITEMS        20
#define   BUFFER_SIZE      10

pthread_mutex_t  buf_mutex        = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  prodcount_mutex  = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  conscount_mutex  = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   space_available  = PTHREAD_COND_INITIALIZER;
pthread_cond_t   data_available   = PTHREAD_COND_INITIALIZER;

int              producer_count, consumer_count;
int              MaxItems = MAX_ITEMS;
int              front    = 0;
int              rear     = 0;
/*----------------------------------------------------------------------------
                                Buffer Object
----------------------------------------------------------------------------*/

int buffer[BUFFER_SIZE];
int buf_count;  /* number of full elements */


void add_buffer(long data)
{
    buffer[rear] = data;
    rear = (rear + 1) % BUFFER_SIZE;
    buf_count++;
}

int get_buffer()
{
    long v;
    v = buffer[front];
    front= (front + 1) % BUFFER_SIZE;
    buf_count--;
    return v ;
}


/*----------------------------------------------------------------------------
                                Thread Functions
----------------------------------------------------------------------------*/

void *producer( void * data)
{
    int i = 0;
    long tid = (long) data;

    while ( ++i <= MaxItems ) {
        pthread_mutex_lock(&buf_mutex);
        while ( BUFFER_SIZE == buf_count )
            pthread_cond_wait(&space_available,&buf_mutex);
        add_buffer( tid*(MaxItems)+i);
        pthread_cond_signal(&data_available);
        pthread_mutex_unlock(&buf_mutex);
    }

    printf("Producer %ld is exiting\n", tid);
    pthread_mutex_lock(&prodcount_mutex);
    producer_count--;
    if ( producer_count == 0 )
        while ( consumer_count > 0 )
            pthread_cond_signal(&data_available);
    pthread_mutex_unlock(&prodcount_mutex);
    pthread_exit(NULL);
}


void *consumer( void * data )
{
    long  tid = (long) data;
    while ( TRUE ) {
        pthread_mutex_lock(&buf_mutex);
        while ( 0 == buf_count ) {
            if ( producer_count > 0 )
                pthread_cond_wait(&data_available,&buf_mutex);
            else {
                pthread_mutex_unlock(&buf_mutex);
                printf("Consumer %ld exiting because all producers left.\n",
                         tid);
                pthread_mutex_lock(&conscount_mutex);
                consumer_count--;
                pthread_mutex_unlock(&conscount_mutex);
                pthread_exit(NULL);
            }
        }
        long v = get_buffer();
        printf("Consumer %ld received %ld from buffer; buffer size = %d\n",
                tid,  v, buf_count);
        pthread_cond_signal(&space_available);
        pthread_mutex_unlock(&buf_mutex);
    }
    pthread_exit(NULL);
}


/*----------------------------------------------------------------------------
                                   Main Program
----------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    char      ch;
    long i;

    char   options[] = "c:p:m:";
    int    numConsumers = 1;
    int    numProducers = 1;
    pthread_t *producer_thread;
    pthread_t *consumer_thread;

    opterr = 0;  /* turn off error messages by getopt() */
    while  (TRUE) {
        ch = getopt(argc, argv, options);
        if ( -1 == ch )
            break;
        switch ( ch ) {
        case 'c':
            if ( VALID_NUMBER != get_int(optarg, NON_NEG_ONLY, &numConsumers,
                 NULL ) )
                usage_error("Invalid argument to -c");
            break;
        case 'p':
            if ( VALID_NUMBER != get_int(optarg, NON_NEG_ONLY, &numProducers,
                 NULL ))
                usage_error("Invalid argument to -p");
            break;
        case 'm':
            if ( VALID_NUMBER != get_int(optarg, NON_NEG_ONLY, &MaxItems,
                NULL ) )
                usage_error("Invalid argument to -m");
            break;
        }
    }
    buf_count = 0;

    producer_thread = (pthread_t*) calloc(numProducers, sizeof (pthread_t));
    consumer_thread = (pthread_t*) calloc(numConsumers, sizeof (pthread_t));
    producer_count = numProducers;
    consumer_count = numConsumers;

    if ( producer_thread == NULL || consumer_thread == NULL )
        fatal_error(errno, "calloc");

    for ( i = 0; i < numConsumers; i++ )
        pthread_create(&consumer_thread[i], NULL, consumer, (void*) i );

    for ( i = 0; i < numProducers; i++ )
        pthread_create(&producer_thread[i], NULL, producer, (void*) i );

    for ( i = 0; i < numProducers; i++ )
        pthread_join(producer_thread[i], NULL);

    for ( i = 0; i < numConsumers; i++ )
        pthread_join(consumer_thread[i], NULL);

    free(producer_thread);
    free(consumer_thread);
    exit(EXIT_SUCCESS);
}



