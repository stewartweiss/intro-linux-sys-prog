/*****************************************************************************
  Title          : test_maxfds.c
  Author         : Stewart Weiss
  Created on     : June 4, 2025
  Description    : Compare actual number of open fds to system limit
  Purpose        : To show how to get system limit and check it
  Usage          : test_maxfds
  Build with     : gcc -Wall -L../lib -I../include -o test_maxfds \
                     test_maxfds.c -lspl

  Note:
  A program starts with 3 open file descriptors, 0, 1, and 2. Therefore,
  we add three after opening as many as we can before the program fails.

  The program executabel must exist, so the program tries to open argv[0]
  instead of some other file.

  When a program exceeds the allowed number of open files, errno is set to
  EMFILE.

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
#include <limits.h>

int main(int argc, char* argv[])
{
    int i = 0;
    int fd;

    errno = 0;
    while ( errno >= 0 ) {
        fd = open(argv[0], O_RDONLY);
        if ( fd == -1 ) {
            if ( errno == EMFILE )
                break;
            else
                fatal_error(errno, "Some problem opening file");
        }
        i++;
    }
    printf("Number of descriptors opened = %d; OPEN_MAX = %ld\n", i+3,
            sysconf(_SC_OPEN_MAX));
    return 0;
}
