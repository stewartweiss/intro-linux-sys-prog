/*****************************************************************************
  Title          : environ_demo.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Displays all environment strings
  Purpose        : To show how to use the environ variable
  Usage          : environ_demo
  Build with     : gcc -o environ_demo environ_demo.c

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

#include <stdlib.h>
#include <stdio.h>

/* Declare the environ variable to be able to access it
   It must be declared extern because it is defined outside
   of the program
*/
extern char **environ;

int main()
{
    char **envp = environ; /* set point to start of list */
    while ( NULL != *envp ) {
        printf("%s\n", *envp );
        envp++;
    }
    return 0;
}
