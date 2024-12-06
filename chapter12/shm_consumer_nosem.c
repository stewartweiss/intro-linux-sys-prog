/*****************************************************************************
  Title          : shm_consumer.c
  Author         : Stewart Weiss
  Created on     : June 8, 2024
  Description    : The producer side of  shared-memory producer/consumer
  Purpose        : To show how to share memory properly
  Usage          : shm_consumer /<shm-name>
  Build with     : gcc -Wall -g -I../include -L../lib  -o shm_consumer \
                    shm_consumer.c -lrt

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

int get_next(sharedbuf* bufpool)
{
    int val;
    val = bufpool->buf[bufpool->front];
    bufpool->front= (bufpool->front + 1) % BUF_SIZE;
    bufpool->count--;
    return val ;
}


int main(int argc, char *argv[])
{
    int fd;
    int val;
    char  usage[256];  /* For error message                                 */

    if ( (argc != 2) || (argv[1][0] != '/') ){
        sprintf(usage, "Usage: %s /shm-name\n", argv[0]);
        usage_error(usage);
    }
    shmpath = argv[1];
     /* Create shared memory object and set its size to the size
        of our structure */
    if ( (fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR,
                       S_IRUSR | S_IWUSR) ) == -1 )
        fatal_error(errno, "shm_open");

    if (ftruncate(fd, sizeof(sharedbuf)) == -1)
        fatal_error(errno,"ftruncate");

     /* Map the object into the caller's address space */
    sharedbuf *shmp = mmap(NULL, sizeof(sharedbuf),
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        fatal_error(errno,"mmap");

    shmp->count = 0;
    shmp->front = 0;
    shmp->rear = 0;
    //sleep(30);

    while ( TRUE ) {
        if ( shmp->count > 0 ) {
            val = get_next(shmp);
            printf("%d\n", val);
        }
    }
     /* Unlink the shared memory object. Even if the peer process
        is still using the object, this is okay. The object will
        be removed only after all open references are closed. */
     shm_unlink(shmpath);
     exit(EXIT_SUCCESS);
 }


