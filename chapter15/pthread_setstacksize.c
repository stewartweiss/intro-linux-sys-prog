/*****************************************************************************
  Title          : pthread_setstacksize.c
  Author         : Stewart Weiss
  Created on     : June 26, 2024
  Description    : Sets the thread stack size of 4 threads
  Purpose        : To show how to set the thread stack size
  Usage          : pthread_setstacksize
  Build with     : gcc -Wall -o pthread_setstacksize -I../include -L../lib \
                   pthread_setstacksize.c  -lspl -lm -lrt -pthread


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


#define NTHREADS 4
#define N 1000
#define MEGEXTRA 1000000

pthread_attr_t attr;

void *dowork(void *threadid)
{
    double A[N][N];
    int i,j;
    long tid;
    size_t mystacksize;

    tid = (long)threadid;
    pthread_attr_getstacksize (&attr, &mystacksize);
    printf("Thread %ld: stack size = %li bytes \n", tid, (long) mystacksize);
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            A[i][j] = ((i*j)/3.452) + (N-i);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NTHREADS];
    size_t    stacksize;
    int       rc;
    long      t;

    pthread_attr_init(&attr);
    pthread_attr_getstacksize (&attr, &stacksize);

    printf("Default stack size = %li\n", (long) stacksize);

    stacksize = sizeof(double)*N*N+MEGEXTRA;
    printf("Amount of stack needed per thread = %li\n",(long) stacksize);

    pthread_attr_setstacksize (&attr, stacksize);
    printf("Creating threads with stack size = %li bytes\n",(long) stacksize);

    for (t=0; t<NTHREADS; t++){
        rc = pthread_create(&threads[t], &attr, dowork, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    printf("Created %ld threads.\n", t);
    pthread_exit(NULL);
}

