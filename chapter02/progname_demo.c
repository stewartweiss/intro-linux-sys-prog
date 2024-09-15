/*****************************************************************************
  Title          : progname_demo.c
  Author         : Stewart Weiss
  Created on     : January  9, 2023
  Description    : Shows how to access command-line arguments
  Purpose        :
  Usage          : progname_demo
  Build with     : gcc -o progname_demo progname_demo.c

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
#include <string.h>

int main(int argc, char * argv[])
{
    char *forwardslashptr;
    char *suffixptr = NULL;

    forwardslashptr = strrchr( argv[0], '/');
    if ( forwardslashptr != NULL )
        suffixptr = forwardslashptr+1;
    else
        suffixptr = argv[0];

    if ( *suffixptr == '\0' )
        fprintf(stderr, "Program name ends in a / character\n");
    else
        printf("Program name is %s\n", suffixptr);
}
