/*****************************************************************************
  Title          : print_args_env.c
  Author         : Stewart Weiss
  Created on     : January  9, 2023
  Description    : Prints  argv and environment strings
  Purpose        : To pass to various exec functions to show how they worK
  Usage          : print_args_env <list of words>
  Build with     : gcc -o print_args_env print_args_env.c

  Notes:
  Because the environment may be large, this limits its output by
  printing only the first few strings from the environment.

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
#define  MAX  3
extern char **environ;

int main( int argc, char* argv[])
{
    int count = 0;
    for (int i = 0; argv[i] != NULL; i++ )
        printf("argv[%d] = %s\n", i, argv[i]);

    char **envp = environ;
    while ( NULL != *envp  && count < MAX ) {
        printf("%s\n", *envp );
        envp++;
        count++;
    }
    return 0;
}
