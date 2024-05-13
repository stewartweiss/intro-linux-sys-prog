/*****************************************************************************
  Title          : execve_demo1.c
  Author         : Stewart Weiss
  Created on     : April 16, 2024
  Description    : Demonstrates how execve() replaces program code
  Purpose        : To demonstrate that an exec system call causes the process
                   to execute a different program. The printf will not be
                   executed unless the execve failed to execute the new
                   program.
  Usage          : execve_demo1 [arg1 arg2 arg3 ...]
                   Runs the echo command displaying the command line arguments

  Build with     : gcc -o execve_demo1 execve_demo1.c


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

#include "common_hdrs.h"

int main (int argc, char * argv[], char * envp[])
{
    if ( argc < 2 )
        usage_error("execve_demo1 options to date\n");

    argv[0] = "date";
    execve("/bin/date", argv, envp);
    fatal_error(errno, "execve");
}
