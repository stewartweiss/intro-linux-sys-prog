/*****************************************************************************
  Title          : sync_io_demo.c
  Author         : Stewart Weiss
  Created on     : April 3, 2006
  Description    : Parent and child synchronize reads/writes from shared file
  Purpose        : To demonstrate how two processes can use signals to
                   synchronize access to a shared file.
  Usage          : sync_io_demo filename
                   where filename is a file that will be opened read/write,
                   truncating  it if it exists.
  Build with     : gcc -Wall -g -I../include -L../lib -o sync_io_demo \
                       sync_io_demo.c -lspl

  Notes          : The program uses SIGUSR1 and SIGUSR2 for synchronizing the
                   processes. SIGUSR1 is sent by each process to the other
                   to indicate that the file can be accessed by the other.
                   SIGUSR2 is sent by writer process (the child) to indicate
                   that it's finished writing and will exit. The parent, on
                   receiving the signal reads what's left in the file.
                   The processes suspend themselves while the other is
                   operating on the file, with pause(). To prevent deadlock
                   the order of pausing and signaling is reversed in each
                   process:
                          kill(parent_pid, SIGUSR1);
                          pause();
                   and the parent,
                          pause();
                          kill(child_pid, SIGUSR1);

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

#define _GNU_SOURCE
#include "common_hdrs.h"

volatile sig_atomic_t  done = 0;

void usr_handler(int sig)
{
    if ( sig == SIGUSR2 )  /* This means child finished. */
        done = TRUE;
    return;
}

void produce_data(int fd )
{
    char           data[] = "abcdefghijklmnoprst:";
    pid_t          ppid;
    static struct  sigaction act;
    int            pos = 0;
    int            count = 0;

    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = usr_handler;
    sigaction(SIGUSR1, &act, NULL); /* To set up signal handler */
    ppid = getppid();               /* Get parent's PID. */

    while ( !done ) {
        usleep(random() % 400000);  /* Delay a random bit to slow program.  */
        lseek(fd, pos, SEEK_SET);   /* Move to saved last position in case
                                       parent moved file offset.            */
        if ( -1 == write(fd, data, strlen(data) ) ) /* Write more data.     */
            fatal_error(errno, "write");
        pos = lseek(fd, 0, SEEK_END); /* Save file offset. */
        if ( ++count < 20 ) {         /* Continue writing. */
            kill(ppid, SIGUSR1);      /* Notify parent to keep reading.     */
            pause();                  /* Wait for signal from parent.       */
        } else {
            kill(ppid, SIGUSR2);      /* Notify parent that writing is done */
            done = TRUE;              /* Exit loop.                         */
        }
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    pid_t          pid;
    const int      CHUNKSIZE = 8;
    static struct  sigaction sigact;
    int            fd;
    char           buffer[CHUNKSIZE+1];
    int            pos = 0;
    size_t         bytes_read;
    if ( argc < 2 )
        usage_error("usage: synch_demo2 filename\n");

    if ( -1 == (fd = open(argv[1], O_CREAT|O_RDWR| O_TRUNC, 0644 )) )
        fatal_error(errno, "open");

    pid = fork();
    if (  -1 == pid )
        fatal_error(errno, "fork");
    else if ( 0 == pid )    /* Child branch. */
        produce_data(fd);   /* Child produces data and writes to file.      */
    else {
        sigact.sa_flags = 0;          /* Install SIGUSR1 handler for parent */
        sigemptyset(&sigact.sa_mask);
        sigact.sa_handler = usr_handler;
        if ( -1 == sigaction(SIGUSR1, &sigact, NULL) )
            fatal_error(errno, "sigaction");
        if ( -1 == sigaction(SIGUSR2, &sigact, NULL) )
            fatal_error(errno, "sigaction");
        while ( TRUE ) {
            pause();
            int unread_bytes = lseek(fd,0,SEEK_END) - pos;
            lseek(fd,pos, SEEK_SET);
            while ( unread_bytes >= CHUNKSIZE ) {
                if ( -1 == ( bytes_read = read(fd, buffer, CHUNKSIZE ) ) )
                    fatal_error(errno, "read");
                if ( bytes_read != CHUNKSIZE )
                    fatal_error(-1, "read");
                unread_bytes -= CHUNKSIZE;
                buffer[CHUNKSIZE] = '\0';
                printf("%s\n", buffer);
            }
            if ( done ) {  /* Child exited, so read last bytes of the file. */
                if ( -1 == read(fd, buffer, unread_bytes) )
                    fatal_error(errno, "read");
                buffer[unread_bytes] = '\0'; /* Null-terminate to print. */
                printf("%s\n", buffer);         /* Print to terminal.       */
                close(fd);                      /* Close descriptor.        */
                exit(EXIT_SUCCESS);             /* Exit.                    */
            }
            else {
                pos = lseek(fd, 0, SEEK_CUR); /* Save position of last read.*/
                kill(pid, SIGUSR1);        /* Signal child to produce data. */
            }
        }
    }
}

