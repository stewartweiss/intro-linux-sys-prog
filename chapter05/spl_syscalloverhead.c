/*****************************************************************************
  Title          : spl_syscalloverhead.c
  Author         : Stewart Weiss
  Created on     : August 1, 2023
  Description    : Call uname sufficiently many times to see cpu usage
  Purpose        : To measure CPU usage of library call
  Usage          : time -p  spl_syscalloverhead
  Build with     : gcc -Wall -g -o spl_syscalloverhead  spl_syscalloverhead.c

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

#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h>

int main(int argc, char* argv[])
{
    int i;
    struct utsname data;

    for ( i = 1; i < 100000000; i++ )
        uname(&data);
    return 0;
}
