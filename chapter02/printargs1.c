/*****************************************************************************
  Title          : printargs.c
  Author         : Stewart Weiss
  Created on     : January  9, 2023
  Description    : Shows how to access command-line arguments
  Purpose        :
  Usage          : printargs <list of words>
  Build with     : gcc -o printargs printargs.c

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

int main(int argc, char *argv[])
{
    printf("%s arguments:\n", argv[0]);
    for ( int i = 1; i < argc ; i++ ){
         printf("%d: %s\n", i, argv[i]);
    }

    printf("\n");
    return 0;
}
