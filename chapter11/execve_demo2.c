/*****************************************************************************
  Title          : execve_demo2.c
  Author         : Stewart Weiss
  Created on     : April 18, 2024
  Description    : Demonstrates how execve() replaces program code
  Purpose        : To demonstrate that an exec system call causes the process
                   to execute a different program.
  Usage          : execve_demo2 [arg1 arg2 arg3 ...]
                   Runs the echo command displaying the command line arguments
  Build with     : gcc -o execvedemo2 execvedemo2.c

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
    if ( argc < 2 )
        usage_error("execve_demo1: program-to-execute [arguments]");

    char* new_env[] = {"AUTHOR=stewart", "CHAPTER=Process Creation", NULL};
    char *path = strdup(argv[1]);
    argv[1] = basename(argv[1]);
    printf("Program being executed is %s.\n",     argv[1]);
    execve(path, argv+1, new_env);
    fatal_error(errno, "execve() failed to run.\n");
}
