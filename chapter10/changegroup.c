/*****************************************************************************
  Title          : changegroup.c 
  Author         : Stewart Weiss
  Created on     : March 15, 2024
  Description    : Changes the process group of the calling program
  Purpose        : To show how to use setpgid() and getpgid()
  Usage          : changegroup 
  Build with     : gcc -Wall -g -o changegroup -I../include -L../lib \
                   changegroup.c -lspl

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


int main( int argc, char *argv[])
{

    printf("PID = %d, GPID = %d\n", getpid(), getpgrp());
    pid_t parent = getppid();
    setpgid(0,parent);

    printf("PID = %d, GPID = %d\n", getpid(), getpgrp());
    setpgid(0,0);
    printf("PID = %d, GPID = %d\n", getpid(), getpgrp());
    return 0;
}
