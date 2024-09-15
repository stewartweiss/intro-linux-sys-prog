/*****************************************************************************
  Title          : str2int.c
  Author         : Stewart Weiss
  Created on     : January  3, 2023
  Description    : Converts its only argument to an integer
  Purpose        : To demonstrate how to use sscanf() for conversions
  Usage          : str2int
  Build with     : gcc -o str2int str2int.c

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char* argv[])
{
    int x;

    if ( argc < 2 ) {
        fprintf(stderr,"usage: %s <number>\n", argv[0]);
        exit(1);
    }
    sscanf(argv[1], " %d", &x);
    printf("The number is %d\n", x);
    return 0;
}
