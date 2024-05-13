/*****************************************************************************
  Title          : execvp_demo.c
  Author         : Stewart Weiss
  Created on     : April 18, 2024
  Description    : Demonstrates how execve() replaces program code
  Purpose        : To demonstrate that an exec system call causes the process
                   to execute a different program.
  Usage          : execvp_demo [arg1 arg2 arg3 ...]
                   Runs the echo command displaying the command line arguments
  Build with     : gcc -o execvp_demo execvp_demo.c

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
    char* p;
    if ( argc < 5 )
         usage_error("execvp_demo command arg1 arg2 arg3\n");
    if (NULL == ( p = strrchr( argv[1], '/')) )
        execvp(argv[1], argv+1);
    else {
        argv[1]= p+1;
        execvp(argv[1],argv+1);
    }
    fatal_error(errno, "execvlp");
}
