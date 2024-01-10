/*****************************************************************************
  Title          : strcoll_demo.c
  Author         : Stewart Weiss
  Created on     : February  22, 2023
  Description    : Prints min of 2 or more strings in the current locale
  Purpose        : To show how to use strcoll()
  Usage          : strcoll_demo string1 string2 ...
  Build with     : gcc -Wall -I../include -L../lib strcoll_demo.c \
                    -o strcoll_demo -lspl

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

int main( int argc, char* argv[])
{
    char *smallest;
    char usage_msg[256];
    int  i = 1, j;

    if ( argc < 3 ) {
        sprintf(usage_msg, "%s string string ...\n", basename(argv[0]));
        usage_error(usage_msg);
    }
    if ( NULL == setlocale(LC_COLLATE, "") )
        fatal_error( LOCALE_ERROR, "setlocale() could not set the "
                    "given locale");

    smallest = argv[i];
    for ( j = i+1; j < argc; j++)
        if ( strcoll(smallest,argv[j]) > 0 )
            smallest = argv[j];
    printf("%s\n",smallest);
    return 0;
}
