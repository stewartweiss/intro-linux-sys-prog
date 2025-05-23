/*****************************************************************************
  Title          : stdin_demo.c
  Author         : Stewart Weiss
  Created on     : January 26, 2006
  Description    : Copies standard input to output a char at a time
  Purpose        : To demonstrate typical text I/O in C
  Usage          : stdin_demo
  Build with     : gcc -o stdin_demo stdin_demo.c
  Modifications  : Added this file to the repository (5/2025)
  Notes          :
         In this program, the return value of getc(stdin) is stored
         into an int variable. This needs explanation.

         getc(stdin) returns an int, not a char. On success, the return
         value is the code for the character it found in the standard
         input stream. That code can be treated like a char in the
         subsequent putchar(), which will write the actual character,
         not its numeric code.

         When the end of input stream is detected by getc(stdin), which
         can be caused by the user's typing Ctrl-D, getc(stdin) returns
         EOF, which is a macro defined in the stdio.h header file to
         be -1.

         On some UNIX systems chars are stored as signed values, from
         -128 to +127. On others, they are stored as unsigned values,
         from 0 to 255. If the variable were to be declared a char and
         chars are unsigned, then when the EOF is returned by getchar,
         the -1 will be stored in an unsigned byte, and the result
         will be the number 255, not equal to EOF in the while loop
         condition.

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

/* Copy from stdin to stdout */
int main()
{
    int c;
    while ( (c = getc(stdin) ) != EOF )
        putc(c, stdout);
    return 0;
}
