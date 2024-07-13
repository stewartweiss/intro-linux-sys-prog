/*****************************************************************************
  Title          :
  Author         : Stewart Weiss
  Created on     :
  Description    :
  Purpose        :
  Usage          :
  Build with     :

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
#include "shm_prodcons.h"

char*  shmpath;
sharedbuf *shmp;

int get_next(sharedbuf* bufpool)
{
    int val;
    WAIT(&bufpool->filledbuf_count);
    WAIT(&bufpool->mutex);
    val = bufpool->buf[bufpool->front];
    bufpool->front= (bufpool->front + 1) % BUF_SIZE;
    POST(&bufpool->mutex);
    POST(&bufpool->emptybuf_count);
    return val ;
}

void cleanup(int signo )
{
    sem_destroy(&shmp->mutex);
    sem_destroy(&shmp->filledbuf_count);
    sem_destroy(&shmp->emptybuf_count);
    shm_unlink(shmpath);
    exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{
    struct sigaction sigact;
    int fd;
    int val;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s /shm-path\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    shmpath = argv[1];
     /* Create shared memory object and set its size to the size
        of our structure */
    if ( (fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR,
                       S_IRUSR | S_IWUSR) ) == -1 )
        fatal_error(errno, "shm_open");
    if (ftruncate(fd, sizeof(sharedbuf)) == -1)
        fatal_error(errno,"ftruncate");

     /* Map the object into the caller's address   space */
    shmp = mmap(NULL, sizeof(sharedbuf),
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        fatal_error(errno,"mmap");

    /* Set up signal handler */
    sigemptyset(&sigact.sa_mask);
    sigact.sa_handler = cleanup;
    sigact.sa_flags = 0;
    if ( ((sigaction(SIGINT,  &sigact, NULL)) == -1 ) ||
         ((sigaction(SIGHUP,  &sigact, NULL)) == -1 ) ||
         ((sigaction(SIGQUIT, &sigact, NULL)) == -1)  ||
         ((sigaction(SIGTERM, &sigact, NULL)) == -1)     )
         fatal_error(errno,"sigaction");

     /* Initialize semaphores as process-shared. */
    INITSEM(&shmp->filledbuf_count,0);
    INITSEM(&shmp->emptybuf_count,BUF_SIZE);
    INITSEM(&shmp->mutex,1);

    shmp->front = 0;
    shmp->rear = 0;

    while (TRUE) {
        val = get_next(shmp);
        printf("%d\n", val);
        usleep(random() % 10000000 );
    }
    cleanup( 1 );
    exit(EXIT_SUCCESS);
}


