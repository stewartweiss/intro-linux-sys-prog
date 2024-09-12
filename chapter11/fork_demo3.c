/****************************************************************************
  Title          : fork_demo3.c
  Author         : Stewart Weiss
  Created on     : Apr. 12, 2024
  Description    : Creates a small file with output from parent and child
  Purpose        : To demonstrate that the new process gets copies of the
                   file descriptors and that both can access the same file.

  Usage          : fork_demo3
  Build with     : gcc -Wall -g -o fork_demo3 fork_demo3.c

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

int main(int argc, char* argv[])
{
    int fd, i;
    pid_t  retval;

    if ( -1 == (fd = open("newfile", O_CREAT|O_WRONLY|O_TRUNC, 0644) ))
        fatal_error(errno, "open");

    if ( -1 == ( retval = fork() ) )
        fatal_error(errno, "fork");

    else if ( 0 == retval )  /* Child executes this branch. */
        for ( i = 0; i < 10; i++) {
            if ( 0 >= write(fd,"c", 1) )
                fatal_error(errno, "write");
            usleep(300000);
        }
    else  /* Parent executes this branch */
        for ( i = 0; i < 10; i++) {
            if ( 0 >= write(fd,"p", 1) )
                fatal_error(errno, "write");
            usleep(200000);
        }
    write(fd, "\0",1);
    close(fd);
    if ( retval != 0 ) printf("File \"newfile\" is ready for viewing.\n");
return 0;
}
