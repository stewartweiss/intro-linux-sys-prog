/****************************************************************************
  Title          : checksyscalls.c
  Author         : Stewart Weiss
  Created on     :
  Description    :
  Purpose        :
  Usage          :
  Build with     : gcc -L../lib  -o checksyscalls checksyscalls.c -lspl
  Modifications  :

*****************************************************************************

Copyright (C) 2023 - Stewart Weiss

This code is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

****************************************************************************/
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include "common_hdrs.h"

int main(int argc, char* argv[])
{
    int fd;
    char buf[32];
    struct stat sb;

    if ( argc < 2 )
        printf("usage: %s file\n", argv[0]);
    else {
        errno = 0;
        if ( -1 == stat(argv[1], &sb) )
            fatal_error(errno,"stat");
        if ( S_ISDIR(sb.st_mode))
            printf("This file is a directory.\n");

        errno = 0;
        fd = open(argv[1], O_RDONLY);
        if ( -1 == fd )
            fatal_error(errno, "open");
        printf("open() was successful.\n");

        errno = 0;
        if ( -1 == read(fd, buf, 1) )
            perror("read() not successful");
        else
            printf("read() was successful.\n");

        errno = 0;
        if ( -1 == close(fd) )
            fatal_error(errno, "close");
        printf("close() was successful.\n");
    }
    return 0;
}
