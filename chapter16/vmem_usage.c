/*****************************************************************************
  Title          : vmem_usage.c
  Author         : Stewart Weiss
  Created on     : June 28, 2024
  Description    : Prints the sum of virtual memory usage of all processes
  Purpose        : To show how to use barrier synchronization in a
                    multithreaded program.
  Usage          : vmem_usage

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
#include <dirent.h>
#include <pthread.h>

#define MAX_LINE    512

long       *partial_sum;  /* array of partial sums of data           */
long       *vmsizes;      /* dynamically allocated array of data     */
pthread_barrier_t barrier; /* Barrier for threads to synchronize     */

/* Data structure passed to each thread start function */
typedef struct _task_data
{
    int first;       /* Index of first element for thread */
    int last;        /* index of last element for thread  */
    int task_id;     /* Thread's program ID               */
    int num_threads; /* Total number of threads           */
    long *data;
} task_data;


/* Extracts the value of vm size from line in /proc/[pid]/stat file.        */
void extract_vmsize_from_buffer(char* buf, long int *vsize)
{
    sscanf(buf, " %*d %*s %*c %*d  %*d " /* pid, comm, state, ppid, pgrp    */
                " %*d %*d "         /* session, tty_nr                      */
                " %*d %*u %*u "     /* skipping tty_pgrp, flags, min_flt    */
                " %*u  %*u %*u "    /* skipping cmin_flt, maj_flt, cmaj_flt */
                " %*u %*u "         /* utime, stime,                        */
                " %*d %*d "         /* skipping cutime, cstime              */
                " %*d %*d "         /* priority, nice                       */
                " %*d %*d "         /* skipping num_threads, alarm          */
                " %*u %lu",         /* start_time, vsize                    */
                                    /* skipping everything after vsize      */
            vsize );
}

/* Gets the virtual memory size in /proc/[pid]/stat and stores in vmdata[i].*/
void get_vmsize( const struct dirent *direntp, int i, long *vmdata )
{
    char    pathname[PATH_MAX];   /* Pathname to file to open               */
    size_t  len = MAX_LINE;       /* Length of line getline() returned      */
    FILE*   fp;                   /* File stream to read                    */
    char*   buf;
    long int vsize;

    if ( NULL == (buf = calloc(MAX_LINE, 1)))/*Allocate buffer for getline()*/
        fatal_error(errno, "calloc");

    memset(pathname, '\0', PATH_MAX);
    sprintf(pathname, "/proc/%s/stat",direntp->d_name);
    if ( NULL == (fp = fopen(pathname, "r") ))
        fatal_error(errno, "fopen");     /* Error opening stat file */
    if ( -1 == getline(&buf,&len, fp ) )
        fatal_error(errno, "getline()");
    extract_vmsize_from_buffer(buf, &vsize);
    vmdata[i] = vsize/1024;               /* Convert to KB */
    free(buf);
}

/* Selects directories whose names are strictly numeric. */
int numeric_dir_filter(  const struct dirent *direntp)
{
    char*   accepts="0123456789";
    if ( (direntp->d_type  == DT_DIR)
        &&  (strspn(direntp->d_name, accepts) == strlen(direntp->d_name)) )
        return 1;
    else
        return 0;
}

/* Creates an array containing the virtual memory sizes of all processes. */
void get_all_vmsizes(long **array, long *n)
{
    struct dirent **namelist;
    long i = 0;

    errno = 0;
    if ( ((*n) = scandir("/proc", &namelist, numeric_dir_filter, NULL) ) < 0)
        fatal_error(errno, "scandir");

    *array = (long*) calloc((*n), sizeof(long));
    if ( *array == NULL )
        fatal_error(errno, "malloc");
    while (i < (*n) - 1) {
        get_vmsize(namelist[i], i, *array );
        free(namelist[i]);
        i++;
    }
    free(namelist);
}


/* Thread start function --- adds the elements of thread_data->data. */
void  *sum_reduce( void * thread_data )
{
    int   half;
    int   retval;

    task_data *t_data  = (task_data*) thread_data;
    int tid            = t_data->task_id;

    partial_sum[tid] = 0;
    for ( int k = t_data->first; k <= t_data->last; k++ )
        partial_sum[tid] += t_data->data[k];

    half = t_data->num_threads;
    while ( half > 1 ) {
        retval = pthread_barrier_wait(&barrier);
        if ( PTHREAD_BARRIER_SERIAL_THREAD != retval &&
             0 != retval )
           pthread_exit((void*) 0);

        if ( half % 2 == 1 && tid == 0 )
            partial_sum[0] +=  partial_sum[half-1];
        half = half/2; // integer division
        if ( tid < half )
            partial_sum[tid] +=  partial_sum[tid+half];
    }
    pthread_exit((void*) 0);

}

/* Compute the sum of values[0]...values[size-1] with num_threads threads. */
long compute_sum( long *values, int size, int num_threads )
{
    int         retval;
    int         t;
    pthread_t   *threads;
    task_data   *thread_data;
    pthread_attr_t attr;
    long        sum;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   /* Allocate the array of threads, task_data structures, data and sums */
    threads     = calloc( num_threads, sizeof(pthread_t));
    thread_data = calloc( num_threads, sizeof(task_data));
    partial_sum = calloc( num_threads, sizeof(double));

    if ( threads == NULL || thread_data == NULL || partial_sum == NULL )
        fatal_error(errno, "calloc");

    /* Initialize a barrier with a count equal to the number of threads. */
    pthread_barrier_init(&barrier, NULL, num_threads);

    for ( t = 0 ; t < num_threads; t++) {
        thread_data[t].first     = (t*size)/num_threads;
        thread_data[t].last      = ((t+1)*size)/num_threads -1;
        thread_data[t].task_id   = t;
        thread_data[t].num_threads = num_threads;
        thread_data[t].data      = values;
        retval = pthread_create(&threads[t], &attr, sum_reduce,
                            (void *) &thread_data[t]);
        if ( retval )
            fatal_error(retval, " pthread_create");
    }
    /* Join all threads so that we can add up their partial sums */
    for ( t = 0 ; t < num_threads; t++) {
        pthread_join(threads[t], (void**) NULL);
    }
    pthread_barrier_destroy(&barrier);
    sum = partial_sum[0];
    free ( threads );
    free ( thread_data );
    free ( partial_sum );
    return ( sum);
}


int main( int argc, char *argv[])
{
    long        array_size;
    long        sum;
    int         retval;
    int         num_threads; /* number of threads this program will use */

    if ( argc < 2 )
        usage_error("this prog num_threads");

    if ( VALID_NUMBER != (retval = get_int(argv[1], NON_NEG_ONLY,
        &num_threads, NULL )))
        fatal_error(retval, "get_int");

    if ( 0 >= num_threads  )
        fatal_error(-1, "Negative number of threads");

    get_all_vmsizes(&vmsizes, &array_size);

    sum = compute_sum(vmsizes, array_size, num_threads);
    printf("%-10ld KB\n", sum);
    free ( vmsizes );
    return 0;
}
