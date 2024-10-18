/*****************************************************************************
  Title          : popen_demo.c
  Author         : Stewart Weiss
  Created on     : May 29, 2024
  Description    : Simulates a shell pipe between two processes using popen
  Purpose        :
  Usage          : popen_demo  command command
  Build with     : gcc -Wall -I../include -L../lib -g -o popen_demo \
                          popen_demo.c -lspl

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
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int    nbytes;             /* Number of bytes read from pipe            */
    FILE  *fin;                /* Stream pointing to read end of pipe       */
    FILE  *fout;               /* Stream pointing to write end of pipe      */
    char   buffer[PIPE_BUF];   /* Buffer for transferring data              */
    int   status1, status2;    /* For collecting status of processes        */

    if ( argc < 3 )
        usage_error("Usage: popen_demo  command1 command2\n");

    if ( (fin = popen(argv[1], "r")) == NULL )
        fatal_error(errno, "popen");

    if ( (fout = popen(argv[2], "w")) == NULL )
        fatal_error(errno, "popen");

    /* Read the output of first command through the descriptor for fin
       and write that output into the descriptor for fout, the write end
       of the pipe that the second command reads as its input.              */
    while ( (nbytes = read(fileno(fin), buffer, PIPE_BUF)) > 0 )
        if ( -1 == write(fileno(fout), buffer, nbytes) )
            fatal_error(errno, "write");
    if ( nbytes < 0 )
        fatal_error(errno, "read");

    status1 = pclose(fin);
    status2 = pclose(fout);
    if (status1 == -1 || status2 == -1 )
        fatal_error(errno, "pclose");
    if (WIFSIGNALED(status1) )
        printf("'%s' terminated by signal %d\n",argv[1], WTERMSIG(status1));
    if (WIFSIGNALED(status2) )
        printf("'%s' terminated by signal %d\n",argv[2], WTERMSIG(status2));
    return 0;
}

