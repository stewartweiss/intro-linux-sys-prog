/*****************************************************************************
  Title          : getpid_demo.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Shows the process id of the calling process
  Purpose        : To demonstrate a first system call
  Usage          : getpid_demo
  Build with     : gcc -o getpid_demo getpid_demo.c

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
#include <unistd.h>
int main()
{
    printf("I am the process with process-id %d\n", getpid());
}
