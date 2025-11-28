/*****************************************************************************
  Title          : unnamedsem_demo.c
  Author         : Stewart Weiss
  Created on     : June 9, 2024
  Description    : To introduce unnamed semaphores
  Purpose        : To show how to use POSIX unnamed semaphore
  Usage          : unnamedsem_demo
  Build with     : gcc -Wall -9 -o unnamedsem_demo -I../include -L../lib \
                     unnamedsem_demo.c -lspl -lpthread
  Note:
  Comment out the semaphore operations and run this program again
  to see why semaphores are necessary.
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
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#define  ITERATIONS   1000000

#define WAIT(Sem)   if ( -1 == sem_wait(Sem)) fatal_error(errno, "sem_wait")
#define POST(Sem)   if ( -1 == sem_post(Sem)) fatal_error(errno, "sem_post")
#define INITSEM(S)  if (sem_init(S,1,1) == -1) fatal_error(errno,"sem_init")

typedef struct shmbuf {
   sem_t  sem;                  /* POSIX unnamed semaphore */
   size_t count;                /* A counter */
} sharedmem;

char*  shmpath   = "/SHMDEMO";  /* Shared memory object name */
sharedmem *shmp;                /* Pointer to shared memory */

int main(int argc, char *argv[])
{
    int fd, i;

     /* Create shared memory object and set its size. */
    if ( (fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR,
                       S_IRUSR | S_IWUSR) ) == -1 )
        fatal_error(errno, "shm_open");
    if (ftruncate(fd, sizeof(sharedmem)) == -1)
        fatal_error(errno,"ftruncate");

     /* Map the object into the caller's address space */
    shmp = (sharedmem*) mmap(NULL, sizeof(sharedmem),
                PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (shmp == MAP_FAILED)
        fatal_error(errno,"mmap");

     /* Initialize semaphore as process-shared.  */
    INITSEM(&shmp->sem);

    shmp->count = 0;  /* Set count to 0 before creating a child process. */
    switch( fork()) {
    case -1:
        fatal_error(errno, "fork");
    case 0:                /* Child process */
        for ( i = 0; i < ITERATIONS; i++ ) {
            WAIT(&shmp->sem);
            shmp->count++;
            POST(&shmp->sem);
        }
        exit(EXIT_SUCCESS);
    default:               /* Parent process */
        for ( i = 0; i < ITERATIONS; i++ ) {
            WAIT(&shmp->sem);
            shmp->count--;
            POST(&shmp->sem);
        }
        wait(NULL);        /* Wait for child to terminate. */
        printf("The final value of count, which should be 0, is %ld.\n",
                shmp->count);
        shm_unlink(shmpath); /* Remove shared memory object. */
        exit(EXIT_SUCCESS);
    }
 }


