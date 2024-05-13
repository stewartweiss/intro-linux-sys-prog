/*****************************************************************************
  Title          : execlp_demo.c
  Author         : Stewart Weiss
  Created on     : April 16, 2024
  Description    : Runs "ls -l" on the program's argument
  Purpose        : To demonstrate that an execl system call causes the process
                   to execute a different program.
  Usage          :

  Build with     : gcc -o execlp_demo execlp_demo.c

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

int main (int argc, char* argv[])
{
    char *p;

    if ( argc < 5 )
         usage_error("execl_demo command arg1 arg2 arg3\n");
    if (NULL == ( p = strrchr( argv[1], '/')) )
        execlp(argv[1], argv[1], argv[2] , argv[3], argv[4], (char *) NULL);
    else
        execlp(argv[1], p+1, argv[2] , argv[3], argv[4], (char *) NULL);

    fatal_error(errno, "execlp");
}




