/*****************************************************************************
  Title          : execle_demo.c
  Author         : Stewart Weiss
  Created on     : April 18, 2024
  Description    : Demonstrates how execve() replaces program code
  Purpose        : To demonstrate that an exec system call causes the process
                   to execute a different program.
  Usage          : execle_demo1  command arg1 arg2 arg3
                   Runs the  command on the command line arguments
  Build with     : gcc -o execle_demo execle_demo.c

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

int main (int argc, char * argv[] )
{
    if ( argc < 5 )
        usage_error("execl_demo command arg1 arg2 arg3\n");
    char* new_env[] = {"AUTHOR=stewart", "CHAPTER=Process Creation", NULL};
    char *path = strdup(argv[1]);
    char* file = basename(path);
    execle(argv[1], file, argv[2], argv[3], argv[4], (char*) NULL, new_env);
    fatal_error(errno, "execve() failed to run.\n");
}
