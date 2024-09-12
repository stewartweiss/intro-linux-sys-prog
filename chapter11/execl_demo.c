/*****************************************************************************
  Title          : execl_demo.c
  Author         : Stewart Weiss
  Created on     : April 16, 2024
  Description    : Runs "ls -l" on the program's argument
  Purpose        : To demonstrate that execlp system call does not need path
  Usage          :
  Build with     : gcc -o execl_demo1 execl_demo1.c

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
#define _GNU_SOURCE
#include "common_hdrs.h"

int main (int argc, char* argv[])
{
    if ( argc < 5 )
         usage_error("execl_demo command arg1 arg2 arg3\n");
    char *path = strdup(argv[1]);
    char* filename = basename(path);
    execl(argv[1], filename, argv[2] , argv[3], argv[4], (char *) NULL);
    fatal_error(errno, "execl");
}




