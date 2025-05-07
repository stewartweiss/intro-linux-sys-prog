/*****************************************************************************
  Title          : hello.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : A program that performs simple input and output
  Purpose        : To introduce simple I/O
  Usage          : hello
  Build with     : gcc -o hello hello.c
  Modifications  :

******************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
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

int main()
{
    char    name[256];
    printf("Enter your name: ");
    scanf("%255s", name);
    printf("hello %s\n", name);
    if ( strlen(name) < 60 )
        exit(1);
}
