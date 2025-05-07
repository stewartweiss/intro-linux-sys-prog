

/****************************************************************************
  Title          : direnttest.c
  Author         : Stewart Weiss
  Created on     : December 10, 2023
  Description    : Tests whether the _DIRENT_HAVE_D_TYPE macro is enabled
  Purpose        : To show how to use the _DIRENT_HAVE_D_TYPE macro.
  Usage          :  <dir>
  Build with     : gcc -Wall -g -I ../include direnttest.c -o direnttest \
                   -L../lib -lspl

*****************************************************************************

Copyright (C) 2025 - Stewart Weiss

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

#define _BSD_SOURCE
#include "../include/common_hdrs.h"
#include <dirent.h>


int main(int argc, char *argv[])
{

    DIR            *dir;
    struct dirent  *entry;
    //int            done = 0;

    if ( argc < 2 )
        return 1;

    dir = opendir(argv[1]);
    while ( 1 ) {
        errno = 0;
        if ( NULL == (entry = readdir(dir) ) ) {
            if ( errno != 0 )
                exit(EXIT_FAILURE);
            else
                break;
        }
        printf("%s\t", entry->d_name);
#ifdef _DIRENT_HAVE_D_TYPE
        switch ( entry->d_type ) {
            case DT_BLK:     printf("block special\n"); break;
            case DT_CHR:     printf("character special\n"); break;
            case DT_DIR:     printf("directory\n"); break;
            case DT_FIFO:    printf("FIFO\n"); break;
            case DT_LNK:     printf("symbolic link\n"); break;
            case DT_REG:     printf("regular\n"); break;
            case DT_SOCK:    printf("socket\n"); break;
            case DT_UNKNOWN: printf("unknown\n"); break;
            default:         printf("\n");
         }
#else
        printf("\n");
#endif
    }
    return 0;
}
