/*****************************************************************************
  Title          : fsidtest.c
  Author         : Stewart Weiss
  Created on     : Sept. 12, 2023
  Description    : Prints out filesystem id for given input file
  Purpose        : To test if statvfs struct's filesystem ID is correct
  Usage          : fsidtest  <file>
  Build with     : gcc -Wall -g -I ../include fsidtest.c -o fsidtest \
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
#include <sys/stat.h>
#include <sys/statvfs.h>
#include "common_hdrs.h"

int main(int argc, char **argv)
{
    struct statvfs statvfs_buffer;

    if (argc < 2)
        printf("usage: this-program pathname\n");
    else
        if ( statvfs(argv[1],  &statvfs_buffer) == 0 )
            printf("Filesystem ID: %lx\n", statvfs_buffer.f_fsid);
    return 0;
}
