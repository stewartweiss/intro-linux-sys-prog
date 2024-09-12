/*****************************************************************************
  Title          : showbreakdiff.c
  Author         : Stewart Weiss
  Created on     : March 20, 2024
  Description    : Shows the virtual addresses of the end and program break
  Purpose        : To show that the program break is not equal to the end
  Usage          : showbreak
  Build with     : gcc -Wall -g -o showbreakdiff -I../include/ -L../lib  \
                     showbreakdiff.c -lspl

  Note:
  THis is the same as showbreak.c except that it does not use the  standard
  library functions, so that we can see that it is not because the library
  is loaded after the data end that the break is well after the end.


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
#include <unistd.h>
#include "common_hdrs.h"

extern char etext, edata, end;


int main(int argc, char *argv[])
{
    char label1[] = "Location of program break:";
    char label2[] = "Location of data end     :";
    char label3[] = "Difference in decimal    :";

    void *break_location = sbrk(0); /* Get the current program break */
    char str[20];
    char nl[2] = "\n";

    lltostr((long) break_location, str, 20);
    write(STDOUT_FILENO, label1, sizeof(label1));
    write(STDOUT_FILENO, str, 20 );
    write(STDOUT_FILENO, nl, 2 );

    lltostr((long) &end, str, 20);
    write(STDOUT_FILENO, label2, sizeof(label2));
    write(STDOUT_FILENO, str, 20);
    write(STDOUT_FILENO, nl, 2 );

    long diff = (long) break_location - (long) &end;
    lltostr((long) diff, str, 20);
    write(STDOUT_FILENO, label3, sizeof(label3));
    write(STDOUT_FILENO, str, 20);
    write(STDOUT_FILENO, nl, 2 );
    return 0;
}



