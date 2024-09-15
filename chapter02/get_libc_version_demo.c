/*****************************************************************************
  Title          : get_libc_version_demo.c
  Author         : Stewart Weiss
  Created on     : December 10, 2022
  Description    : Prints current version of GNU C Library
  Purpose        : Show how to get library version at runtime
  Usage          : get_libc_version_demo
  Build with     : gcc -o get_libc_version_demo get_libc_version_demo.c

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

#include <gnu/libc-version.h>
#include <stdio.h>

int main()
{
    printf("The version of glibc is:%s\n",gnu_get_libc_version());
    return 0;
}
