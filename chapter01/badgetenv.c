/******************************************************************************
  Title          : bad_getenv_demo.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Will not compile
  Purpose        : To show an incorrect way to call getenv()
  Usage          : Cannot use this
  Build with     : gcc -o bad_getenv_demo bad_getenv_demo.c
  Modifications  : Replaced COLUMNS by SHELL

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

int main()
{
    char shell[200];
    shell = getenv("SHELL");  /* This cannot compile because shell
                                 is a const char* but getenv() allocates
                                 its own memory for the returned string
                                 and returns its address. */
    printf("The current shell is %s.\n", shell);
};
