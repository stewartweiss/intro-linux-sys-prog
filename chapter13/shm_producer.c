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

void add_next(sharedbuf* bufpool, int data)
{
    WAIT(&bufpool->emptybuf_count);
    WAIT(&bufpool->mutex);
    bufpool->buf[bufpool->rear] = data;
    bufpool->rear = (bufpool->rear + 1) % BUF_SIZE;
    POST(&bufpool->mutex);
    POST(&bufpool->filledbuf_count);
}

int main(int argc, char *argv[])
{
    int fd;
    int val;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s /shm-path\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    shmpath = argv[1];

     /* Open the existing shared memory object and map it
        into the caller's address space */
    if ( (fd = shm_open(shmpath, O_RDWR, 0) ) == -1 )
        fatal_error(errno, "shm_open");

    sharedbuf *shmp = mmap(NULL, sizeof(sharedbuf),
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        fatal_error(errno,"mmap");

    while ( TRUE ) {
        if ( scanf("%d", &val) > 0 ) {
            printf("Sending %d\n", val);
            add_next(shmp, val);
            usleep(random() % 1000000);
        }
        else
            break;

    }
    exit(EXIT_SUCCESS);
}

