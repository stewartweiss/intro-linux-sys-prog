/*****************************************************************************
  Title          : pthread_demo2.c
  Author         : Stewart Weiss
  Created on     : June 24, 2024
  Description    : Computes sum of numbers in an array using threads
  Purpose        : To show how to pass structured arguments to threads
  Usage          : pthread_demo2  number_of_threads size_of_array  [file]

  Build with     : gcc -Wall -o pthread_demo2 -I../include -L../lib \
                   pthread_demo2.c  -lspl -lm -lrt -pthread

  Notes
  This program demonstrates a technique for passing data to cooperating threads
  that work on a shared task. In this case it is computing the sum of the
  values in an array.

  Because the program does not know in advance how much data there is, the
  array is allocated on the heap, with the pointer to it a global variable in
  the process.

  We allow the number of threads to be decided dynamically. In practice, the
  number of threads would be computed based on configuration parameters and
  possibly the data size. If the number of threads is too large for the given
  system, this program will return an error. There is no limit on the number
  of threads per process, but there is on the resources they use (such as stack
  size, pages, etc.)

  Within the main loop, each thread's task_data structure is initialized and
  the thread is created, explicitly setting it as joinable. The array is
  partitioned among the threads by dividing the array size by the number of
  threads and taking the ceiling. The last thread may have less work to do.

  After all threads have exited, the process adds up the sums in the sums array.
  A more efficient algorithm will use a reduction technique, in which the
  threads themselves add the sums in a divide-and-conquer algorithm that
  reduces the runtime complexity to O(N/t +log(t)), where  is the size of the
  array, t is the number of threads. See thread_barrier_demo1.c for this
  solution.

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

#define  _GNU_SOURCE
#include "common_hdrs.h"
#include <pthread.h>
#include <locale.h>
#include <math.h>

/*****************************************************************************
                             Data Types and Constants
*****************************************************************************/


double       *sum;        /* array of partial sums of data           */
int          num_threads; /* number of threads this program will use */


/*
   a task_data structure contains the data required for a thread to compute
   the sum of the segment of the array it has been delegated to total, storing
   the sum in its cell in an array of sums. The data array and the sum array
   are allocated on the heap. The threads get the starting addresses of each,
   and their task number and the first and last entries of their segments.
*/
typedef struct _task_data
{
    pthread_t tid;
    int first;       /* index of first element for task */
    int last;        /* index of last element for task */
    int task_id;     /* id of thread */
    double *array;   /* Start of array in heap */
} task_data;


/*****************************************************************************
                         Thread and Helper Functions
*****************************************************************************/

int get_data(FILE* fp, double *array, int maxsize)
{
    int val;
    int i = 0;
    while ( i < maxsize ) {
        if ( fscanf(fp, "%d", &val) > 0 ) {
            array[i] = val;
            i++;
        }
        else
            break;
    }
    return i;
}

/**
   The thread routine. This computes the sum of the numbers from
   the segment t_data->first to t_data->last and puts that sum
   into the cell t_data->sums[t_data->thread_id]
*/
void*  add_array( void  *thread_data )
{
    task_data *t_data;
    int   k;
    int   thread_id;

    t_data    = (task_data*) thread_data;
    thread_id = t_data->task_id;
    sum[thread_id] = 0;
    for ( k = t_data->first; k <= t_data->last; k++ )
        sum[thread_id] += t_data->array[k];

    pthread_exit((void*) 0);
}

/*****************************************************************************
                               Main Program
*****************************************************************************/

int main( int argc, char *argv[])
{
    int        array_size;
    double     total;
    int        t;
    int        n;
    double    *array;      /* dynamically allocated array of data     */
    char       usage[512];

    task_data  *thread_data;
    pthread_attr_t attr;
    FILE     *fp;

    /* Instead of assuming that the system creates threads as joinable by
       default, this sets them to be joinable explicitly.
    */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    if ( argc < 3 ) {
        sprintf(usage, "%s <num-threads> <array-size>", basename(argv[0]));
        usage_error(usage);
    }

    /* Get command line arguments, convert to ints, and compute size of each
       thread's segment of the array
    */
    num_threads = atoi(argv[1]);
    array_size  = atoi(argv[2]);

    /* Allocate the array of threads, task_data structures, data and sums */
    thread_data = calloc( num_threads, sizeof(task_data));
    array       = calloc( array_size, sizeof(double));
    sum         = calloc( num_threads, sizeof(double));

    if (  thread_data == NULL || array == NULL || sum == NULL )
        fatal_error(errno, "calloc");

    if ( argc >= 4 ) {
        if ( NULL == (fp = fopen(argv[3],"r") ) )
            fatal_error(errno, "fopen");
        n = get_data(fp, array, array_size);
    }
    else
        n = get_data(stdin, array, array_size);

    array_size = n;

    /* Initialize task_data for each thread and then create the thread */
    for ( t = 0 ; t < num_threads; t++) {
        thread_data[t].first     = (t*array_size)/num_threads;
        thread_data[t].last      = (((t+1)*array_size)/num_threads) - 1;
        thread_data[t].task_id   = t;
        thread_data[t].array     = array;
        if ( 0 != pthread_create(&(thread_data[t].tid), &attr, add_array,
                            (void *) &thread_data[t]) )
            fatal_error(errno, "pthread_create");
    }

    /* Join all threads so that we can add up their partial sums */
    for ( t = 0 ; t < num_threads; t++) {
        pthread_join(thread_data[t].tid, (void**) NULL);
    }
    /* Collect partial sums into a final total */
    total = 0;
    for ( t = 0 ; t < num_threads; t++) {
        total += sum[t];
    }
    printf("The array total is %7.2f\n", total);

    /* Free all memory allocated to program */
    free ( thread_data );
    free ( array );
    free ( sum );
    return 0;
}
