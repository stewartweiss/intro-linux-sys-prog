/****************************************************************************
  Title          : testdircalls.c
  Author         : Stewart Weiss
  Created on     : October 8, 2023
  Description    : Tests whether calls to open(), read(), close() cause errors
  Purpose        : To check which calls work with directories
  Usage          : testdircalls <directory-file>
  Build with     : gcc -Wall -g -I ../include testdircalls.c -o testdircalls \
                   -L../lib -lspl
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
