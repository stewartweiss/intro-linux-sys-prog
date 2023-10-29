/******************************************************************************
  Title          : smallest.c
  Author         : Stewart Weiss
  Created on     : February  22, 2023
  Description    : Compares two or more strings in the current locale
  Purpose        : To show how to use strcoll()
  Usage          : smallest string1 string2 ...
  Build with     : gcc -Wall -L../lib smallest.c -o smallest -lspl

 * Copyright (C) 2023 - Stewart Weiss
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/
#define _GNU_SOURCE
#include "common_hdrs.h"

int main( int argc, char* argv[])
{
    char *smallest;
    int  num_elements;
    char usage_msg[256];
    int  i = 1, j;

    if ( argc < 3 ) {
        sprintf(usage_msg, "%s string string ...\n", basename(argv[0]));
        usage_error(usage_msg);
    }
    if ( NULL == setlocale(LC_COLLATE, "") )
        fatal_error (-1,"setlocale");

    num_elements = argc-1; /* Number of words to compare */
    smallest = argv[i];
    for ( j = i+1; j <= num_elements; j++)
        if ( strcoll(smallest,argv[j]) > 0 )
            smallest = argv[j];
    printf("%s\n",smallest);
    return 0;
}
