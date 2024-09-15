/*****************************************************************************
  Title          : getenv_demo.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Displays the value of the HOME environment variable
  Purpose        : To show how to use getenv()
  Usage          : getenv_demo
  Build with     : gcc -o getenv_demo getenv_demo.c

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
#include <string.h>
#include <stdio.h>

int main()
{
    char *path_to_home;
    path_to_home  = getenv("HOME");
    if ( NULL == path_to_home )
        printf("The HOME variable is not in the environment.\n");
    else
        printf("HOME=%s\n", path_to_home);
    return 0;
}
