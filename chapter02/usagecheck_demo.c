/*****************************************************************************
  Title          : usagecheck_demo.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Displays an error message if run with < 2 arguments
  Purpose        : To demonstrate how to use handle argument shortage
  Usage          : usagecheck_demo
  Build with     : gcc -o usagecheck_demo usagecheck_demo.c

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

int main ( int argc,  char * argv[]  )
{
    if ( argc < 3 ) { /* too few arguments */
        /* handle the incorrect usage here */
        fprintf(stderr, "usage: %s file1 file2\n", argv[0]);
        exit(1);
    }
    printf("About to copy from %s to %s\n", argv[1], argv[2]);
    /* but no copying yet */
    return 0;
}
