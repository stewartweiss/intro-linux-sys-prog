
/*****************************************************************************
  Title          : upcopychars2.c
  Author         : Stewart Weiss
  Created on     : June 20, 2024
  Description    : Convert chars to uppercase from standard input to  output
  Purpose        : Demonstrates that buffering/character processing exist
  Usage          : upcopychars2
                   After starting the program type lowercase characters
                   followed by Enter key. Use Ctrl-D to quit
  Build with     : gcc -Wall -L../lib -I../include -o upcopychars2 \
                     upcopychars2.c -lspl

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
#include <ctype.h>

int main( int argc, char *argv[] )
{
    char c;
    while(   fread( &c, 1, 1, stdin) > 0 )
        printf("char = '%c' code = %03d\n", c, c );
    return 0;
}

