/*****************************************************************************
  Title          : shm_prodcons.h
  Author         : Stewart Weiss
  Created on     : June 10, 2024
  Description    : Header file for shared memory  producer/consumer program
  Purpose        : To define shared memory object shared by processes

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
#ifndef _SHM_PRODCONS_H
#define _SHM_PRODCONS_H

#include "common_hdrs.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

#define WAIT(S)     if ( -1 == sem_wait(S)) fatal_error(errno, "sem_wait")
#define POST(S)     if ( -1 == sem_post(S)) fatal_error(errno, "sem_post")
#define INITSEM(S,N) if (-1 ==sem_init(S,1,(N))) fatal_error(errno,"sem_init")

#define BUF_SIZE 512        /* Size of buffer pool                 */

/* The shared  memory object */
typedef struct shmbuf {
   sem_t  filledbuf_count;  /* To  count filled buffers             */
   sem_t  emptybuf_count;   /* To count empty buffers               */
   sem_t  mutex;            /* To prevent race condition on buffer  */
   int    front, rear;      /* Index of next read, write in buffer  */
   int    buf[BUF_SIZE];    /* Stores data being transferred        */
} sharedbuf;

#endif /* _SHM_PRODCONS_H */
