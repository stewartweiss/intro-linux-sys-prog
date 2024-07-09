/*****************************************************************************
  Title          : pipe_demo1.c
  Author         : Stewart Weiss
  Created on     : May 16, 2024
  Description    : Demonstrates a pipe between parent and child.
  Purpose        : To introduce the pipe() system call.
  Usage          : pipe_demo1 <string>
                   To send multiple words, enclose in single quotes
  Build with     : gcc -Wall -I../include -L../lib -g -o pipe_demo1 \
                          pipe_demo1.c -lspl


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

int main(int argc, char* argv[])
{
    if (argc < 2)
        usage_error("Usage: system_demo command ");
    int ret = system(argv[1]);
    if ( ret == -1 )
        fatal_error(errno, "system");
    exit(EXIT_SUCCESS);
}
