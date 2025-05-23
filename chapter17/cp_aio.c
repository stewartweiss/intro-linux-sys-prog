/*****************************************************************************
  Title          : cp_aio.c
  Author         : Stewart Weiss
  Created on     : August 1, 2023
  Description    : Copies a file with asynchronous reads & synchronous writes
  Purpose        : To show an example of the POSIX AIO API
  Usage          : cp_aio sourcefile targetfile
  Build with     : gcc -Wall -g -I../include -L ../lib -lspl \
                   -o cp_aio  cp_aio.c

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

#define BUFFER_SIZE    4096
#define MESSAGE_SIZE   512
#define PERMISSIONS    S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH  /*rw-rw-r-- */
volatile sig_atomic_t  input_ready = 0;

/* The signal handler for SIGIO */
void on_input(int sig, siginfo_t *si, void *ucontext)
{
    input_ready = 1;
}

void setup_handler()
{
    struct  sigaction sigact;

    sigact.sa_sigaction = on_input;
    sigact.sa_flags = SA_RESTART | SA_SIGINFO;
    sigemptyset(&sigact.sa_mask);
    if ( sigaction(SIGIO, &sigact, NULL) == -1 )
        fatal_error(errno, "sigaction");
}

int main(int argc, char *argv[])
{
    int     source_fd;
    int     target_fd;
    int     num_bytes_read;
    int     num_bytes_written;
    mode_t  permissions = PERMISSIONS;
    int     i = 0;
    char    *buf[2];
    char    *writebuf;
    char    message[MESSAGE_SIZE];
    struct  aiocb  aio_block;


    /* Check for correct usage.                                             */
    if ( argc != 3 ){
        sprintf(message,"%s source destination", basename(argv[0]));
        usage_error(message);
    }

    /* Set up signal handling  */
    setup_handler();

    /* Open source file for reading.                                        */
    if ( (source_fd = open(argv[1], O_RDONLY)) == -1 ) {
        sprintf(message, "unable to open %s for reading", argv[1]);
        fatal_error(errno, message);
    }

    /* Open target file for writing.                                        */
    if ( (target_fd = open( argv[2], O_WRONLY|O_CREAT|O_TRUNC,
                            permissions) ) == -1 ) {
        sprintf(message, "unable to open %s for writing", argv[2]);
        fatal_error(errno, message);
    }

    if ( (NULL == (buf[0] = calloc(BUFFER_SIZE,1)))
         || (NULL == (buf[1] = calloc(BUFFER_SIZE,1))) )
        fatal_error(errno, "calloc");

    memset(&aio_block, 0, sizeof(aio_block));
    aio_block.aio_buf     = buf[0];
    aio_block.aio_fildes  = source_fd;
    aio_block.aio_nbytes  = BUFFER_SIZE;
    aio_block.aio_reqprio = 0;
    aio_block.aio_offset  = 0;
    aio_block.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    aio_block.aio_sigevent.sigev_signo  = SIGIO;

    if ( -1 == aio_read(&aio_block))
        fatal_error(errno, "aio_read");

    BOOL done = FALSE;
    while ( !done ){
        if ( input_ready ) {
            input_ready = 0;
            num_bytes_read = aio_return(&aio_block);
            if ( num_bytes_read > 0 ) {
                writebuf = (char*) aio_block.aio_buf;
                i = i^1;
                aio_block.aio_buf     = buf[i];
                aio_block.aio_offset += num_bytes_read;
                if ( -1 == aio_read(&aio_block))
                    fatal_error(errno, "aio_read");
                num_bytes_written = write( target_fd, writebuf, num_bytes_read ) ;
                if ( errno == EINTR )
                   printf("write() was interrupted by read completion\n");
                else if ( errno != 0 ) /* Some other error */
                    fatal_error(errno, "write()");
                else /* Successful write; check if all bytes were written.*/
                    if ( num_bytes_written  != num_bytes_read) {
                        sprintf(message,"write error to %s\n", argv[2]);
                        fatal_error(-1, message);
                    }
            }
            else
                done = TRUE;
        }
    }

    /* Close files.                                                       */
    if ( close(source_fd) == -1 ) {
        sprintf(message, "error closing  %s", argv[1]);
        fatal_error(-1, message);
    }
    if (-1 == fsync(target_fd))  /* Flush data to device. */
        if (errno != EINVAL)     /* If not a terminal */
            fatal_error(errno, "fsync");
    if ( close(target_fd) == -1 ) {
        sprintf(message, "error closing %s", argv[2]);
        fatal_error(-1, message);
    }
    free(buf[0]);
    free(buf[1]);
    return 0;
  }
