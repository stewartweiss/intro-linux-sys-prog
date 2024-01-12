/******************************************************************************
  Title          : hello_world.c
  Author         : Kernighan and Ritchie
  Created on     :
  Description    : A program that prints "hello, world."
  Usage          : hello_world
  Build with     : gcc -o hello_world hello_world.c

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

int main()
{
    char    name[256];
    printf("Enter your name: ");
    scanf("%255s", name);
    printf("hello %s\n", name);
    if ( strlen(name) < 60 )
        exit(1);
}
#include <stdio.h>

void main()
{
    printf("hello, world.\n");
}
