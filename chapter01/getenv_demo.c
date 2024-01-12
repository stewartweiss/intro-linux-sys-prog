/*****************************************************************************
  Title          : getenv_demo.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Displays value of COLUMNS environment variable
  Purpose        : To show one way to get value of an environment variable
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

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char* num_columns = getenv("COLUMNS");
    printf("This window has %s columns.\n", num_columns);
};
