/*****************************************************************************
  Title          : basename_demo.c
  Author         : Stewart Weiss
  Created on     : November 20, 2022
  Description    : Displays the computer name
  Purpose        : To demonstrate how to use basename()
  Usage          : basename_demo
  Build with     : gcc -o basename_demo basename_demo.c

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
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[])
{
    char *progname;

    progname = basename(argv[0]);
    printf("Program name is %s\n", progname);
    return 0;
}
