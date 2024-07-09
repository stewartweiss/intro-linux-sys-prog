/*****************************************************************************
  Title          : pthread_signal_demo.c
  Author         : Stewart Weiss
  Created on     : June 10, 2024
  Description    : A  program that shows effects of signals on threads
  Purpose        : To demonstrate rules of signals and threads
  Usage          : pthread_signal_demo
  Build with     : gcc -Wall -o pthread_signal_demo -I../include -L../lib \
                   pthread_signal_demo.c  -lspl -lm -lrt -pthread

  Notes
    By compiling the program, defining  zero or more of the three
    symbols, MAINSIGNALS, BLOCK1, and SIGHANDLE, such as:
         gcc -I../include -L../lib pthread_signal_demo.c  -o pthread_\
          signal_demo -lspl -lm -lrt -pthread -DBLOCK1  -DSIGHANDLE

    you can run it to confirm the signaling semantics.
    The main() function creates three threads and then waits for them.

    If MAINSIGNALS is defined when you compile it,  {main() will send a
    thread-directed SIGINT signal to thread 1.

    If SIGHANDLE is defined, then a signal handler will be registered by
    thread 2.

    If BLOCK1 is defined, then thread 1 blocks SIGINT.

    If you run it without any of these symbols defined, the three
    threads will exit normally.

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

#ifndef SLEEP
    #define SLEEP 10
#endif

sigset_t  mask;     /* Signal mask */
pthread_t  thr[3];  /* Thread IDs  */

void sighandler( int sig )
{
    printf("Received %d; handler run by thread %ld\n",  sig, pthread_self());
    return;
}

void* t1( void * arg)   /* Thread t1 start function */
{
#ifdef BLOCK1
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
#endif
    sleep(SLEEP);
    pthread_exit((void*) arg);
}

void* t2( void * arg)   /* Thread t2 start function */
{
#ifdef SIGHANDLE
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sighandler;
    sigaction(SIGINT, &sa, NULL);
#endif
    sleep(SLEEP);
    pthread_exit((void*) arg);
}

void* t3( void * arg)   /* Thread t3 start function */
{
    sleep(SLEEP);
    pthread_exit((void*) arg);
}


int main( int argc, char *argv[])
{
    long      *status;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    if ( 0 != pthread_create(&thr[0], NULL, t1,(void*) 1) )
        fatal_error(errno, "pthread_create");
    if ( 0 != pthread_create(&thr[1], NULL, t2,(void*) 2) )
        fatal_error(errno, "pthread_create");
    if ( 0 != pthread_create(&thr[2], NULL, t3,(void*) 3) )
        fatal_error(errno, "pthread_create");

    pthread_sigmask(SIG_BLOCK, &mask, NULL);
    sleep(1);
#ifdef MAINSIGNALS
    printf("Sending SIGINT to thread 1\n");
    pthread_kill(thr[0], SIGINT);
#endif

   /* Wait for the child threads. */
    for ( int i = 0; i < 3; i++) {
        if ( 0 != pthread_join(thr[i], (void**) (&status)) )
            fatal_error(errno, "pthread_join");
        printf("Child exited with status %ld\n", (long) status);
    }
    return 0;
}
