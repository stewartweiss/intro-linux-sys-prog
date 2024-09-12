/*****************************************************************************
  Title          : upcopychars.c
  Author         : Stewart Weiss
  Created on     : June 20, 2024
  Description    : Convert chars to uppercase from standard input to  output
  Purpose        : Demonstrates that buffering/character processing exist
  Usage          : upcopychars
                   After starting the program type lowercase characters
                   followed by Enter key. Use Ctrl-D to quit
  Build with     : gcc -Wall -L../lib -I../include -o upcopychars \
                     upcopychars.c -lspl

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
    char inbuf;
    char prompt[] = "Type any characters followed by ENTER, Ctrl-D to exit.\n";
    if ( -1 == write(1, prompt, strlen(prompt)) )
        fatal_error(errno, "write");
    while( read(STDIN_FILENO, &inbuf, 1) > 0  ) {
        inbuf = toupper(inbuf);
        if ( -1 == write( 1, &inbuf, 1 ))
             fatal_error(errno, "write");
    }
    return 0;
}

