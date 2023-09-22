/******************************************************************************
  Title          : fsidtest.c
  Author         : Stewart Weiss
  Created on     : Sept. 12, 2023
  Description    : Prints out filesystem id for given input file
  Purpose        : To test if statvfs struct's filesystem ID is correct
  Usage          : fsidtest  <file>
  Build with     : gcc -Wall -g fsidtest.c -o fsidtest
  Modifications  :

*******************************************************************************

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

******************************************************************************/
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/statvfs.h>

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
