/*****************************************************************************
  Title          : namedsem_demo.c
  Author         : Stewart Weiss
  Created on     : June 10, 2024
  Description    : Synchronized writes to a shared file
  Purpose        : Show how named semaphores can be used
  Usage          : namedsem_demo <filename>
  Build with     : gcc -Wall -g -o namedsem_demo -I../include -L../lib \
                       namedsem_demo.c  -lspl -lpthread

******************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include "common_hdrs.h"
#include <sys/wait.h>
#include <semaphore.h>       /* For sem_* operations */
#define  ITERATIONS   1000
#define  SIZE         8192
#define  SEMNAME      "/DEMOSEM"

#define  WAIT(S)  if ( -1 == sem_wait(S)) fatal_error(errno, "sem_wait")
#define  POST(S)  if ( -1 == sem_post(S)) fatal_error(errno, "sem_post")

int main( int argc, char* argv[])
{
    sem_t *sem;
    int   status;
    int   i;
    FILE* fp;

    char  str1[SIZE], str2[SIZE];
    memset(str1,'a', SIZE-1);
    memset(str2, 'b', SIZE-1);
    str1[SIZE-1] = '\0';
    str2[SIZE-1] = '\0';
    if ( argc < 2 )
        usage_error("usage: unnamedsem_demo filename\n");

    if ( NULL == (fp = fopen(argv[1], "w" )) )
        fatal_error(errno, "fprintf");

    sem = sem_open(SEMNAME, O_CREAT | O_EXCL, 0660, 1 );
    if ( SEM_FAILED == sem )
        fatal_error(errno, "sem_open");

    switch ( fork() ) {
    case -1:
        fatal_error(errno, "fork");
    case  0:
        for (  i = 0; i < ITERATIONS; i++ ) {
            WAIT(sem);
            fprintf(fp, "%s\n", str1);
            fflush(fp);
            POST(sem);
        }
        exit(EXIT_SUCCESS);
    default:
        for (  i = 0; i < ITERATIONS; i++ ) {
            WAIT(sem);
            fprintf(fp, "%s\n", str2);
            fflush(fp);
            POST(sem);
        }
        wait(&status);
        if ( WIFSIGNALED(status) )
            printf("Child process killed by signal %d\n", WTERMSIG(status));
        fclose(fp);
        sem_unlink(SEMNAME);
        exit(EXIT_SUCCESS);
    }
}
