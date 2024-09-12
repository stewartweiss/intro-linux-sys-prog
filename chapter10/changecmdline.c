/*****************************************************************************
  Title          : changecmdline.c
  Author         : Stewart Weiss
  Created on     : March 15, 2024
  Description    : Changes the command line words of caller
  Purpose        : To show that argv can be changed by process
  Usage          : changecmdline
  Build with     : gcc -Wall -g -o changecmdline -I../include -L../lib \
                   changecmdline.c -lspl

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
#include <stdio.h>
#include <unistd.h>


int main( int argc, char* argv[])
{
    int i;
    char* word = "hello";

    for ( i = 1; i < argc ; i++ )
        argv[i] = word;
    for ( i = 1; i < argc ; i++ )
        printf("%s ", argv[i]);
    printf("\n");
    return 0;
}
