/*****************************************************************************
  Title          : aio_write_demo.c
  Author         : Stewart Weiss
  Created on     : August 1, 2024
  Description    : Monitors progress of aio writes
  Purpose        : To show an example of the aio_error(0 and aio_write()
  Usage          : cp_aio targetfile
  Build with     : gcc -Wall -g -I../include -L ../lib -lspl \
                   -o aio_write_demo  aio_write_demo.c

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

#define _GNU_SOURCE
#include "common_hdrs.h"
#include <aio.h>
#include <fcntl.h>


#define BUFFER_SIZE      64
#define MESSAGE_SIZE   512
#define PERMISSIONS    S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH  /*rw-rw-r-- */
volatile sig_atomic_t  output_ready = 0;


void   on_output(int sig, siginfo_t *si, void *ucontext)
{
    output_ready = 1;
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    printf("%ld.%ld\n", ts.tv_sec, ts.tv_nsec);
}

int main(int argc, char *argv[])
{

    int     target_fd;
    int     num_bytes_read;
    int     num_bytes_written;
    mode_t  permissions = PERMISSIONS;
    char    *buf;
    char    message[MESSAGE_SIZE];
    struct  aiocb  aio_block;
    struct sigaction sigact;
    struct stat  st;
    int total_bytes = 0;
    struct timespec ts;

    /* Check for correct usage.                                             */
    if ( argc != 2 ){
        sprintf(message,"%s destination", basename(argv[0]));
        usage_error(message);
    }

    /* Set up signal handling  */
    sigact.sa_sigaction = on_output;
    sigact.sa_flags = SA_RESTART | SA_SIGINFO;
    sigemptyset(&sigact.sa_mask);
    if ( sigaction(SIGIO, &sigact, NULL) == -1 )
        fatal_error(errno, "sigaction");

    if ( NULL == (buf = calloc(BUFFER_SIZE,1)))
        fatal_error(errno, "calloc");

    /* Read a line from standard input. */
    if ( -1 == (num_bytes_read = read(0, buf, BUFFER_SIZE)))
        fatal_error(errno, "read");

    /* Open target file for writing.                                        */
    if ( (target_fd = open( argv[1], O_WRONLY|O_CREAT|O_TRUNC| O_APPEND,
                            permissions) ) == -1 ) {
        sprintf(message, "unable to open %s for writing", argv[1]);
        fatal_error(errno, message);
    }

    printf("Num bytes read  = %d\n", num_bytes_read);
    memset(&aio_block, 0, sizeof(aio_block));
    aio_block.aio_buf     = buf;
    aio_block.aio_fildes  = target_fd;
    aio_block.aio_nbytes  = num_bytes_read/2;
    aio_block.aio_reqprio = 0;
    aio_block.aio_offset  = 0;
    aio_block.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    aio_block.aio_sigevent.sigev_signo  = SIGIO;

    if ( -1 == aio_write(&aio_block))
        fatal_error(errno, "aio_read");

      errno = 0;
      int done = 0;
      while ( !done ){
          if ( output_ready ) {
              output_ready = 0;
              fstat(target_fd, &st);
              printf("File size = %ld, mtime = %ld.%ld\n",
                      st.st_size, st.st_mtim.tv_sec, st.st_mtim.tv_nsec);
              num_bytes_written = aio_return(&aio_block);

              if ( num_bytes_written >= 1 ) {
                  total_bytes += num_bytes_written;
                  printf("Bytes written = %d\n", num_bytes_written);
                  if ( total_bytes < num_bytes_read)  {
                      aio_block.aio_buf += num_bytes_written;
                      if ( -1 == aio_write(&aio_block))
                          fatal_error(errno, "aio_read");
                      }
                  else
                      done = 1;
              }
              else
                  done = 1;
          }
         int s = aio_error(&aio_block);
         switch (s) {
         case 0:
             break;
         case EINPROGRESS:
             printf("In progress\n");
             break;
         case ECANCELED:
             printf("Canceled\n");
             break;
         default:
             perror("aio_error");
             break;
         }
      }

    /* Close file.                                                       */
    if ( close(target_fd) == -1 ) {
        sprintf(message, "error closing target file %s", argv[2]);
        fatal_error(errno, "error closing target file");
    }
    free(buf);
    return 0;
  }
