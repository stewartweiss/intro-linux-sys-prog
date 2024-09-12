/*****************************************************************************
  Title          : fillqueue.c
  Author         : Stewart Weiss
  Created on     : July 14, 2024
  Description    : Simulates writing characters into the terminal input
  Purpose        : To show that ioctl() can be used to insert characters into
                   a terminal's input queue
  Usage          : ioctl2
  Build with     : gcc -Wall -L../lib -I../include -o fillqueue \
                     fillqueue.c -lspl

  Notes:
  This is designed to be used in conjunction with watchtty.c, which reads the
  number of characters in a terminal queue and write it to a file.
  It inserts characters into the input queue after an initial delay,
  which gives you time to start up the second program.  Then it reads the
  characters from the terminal with a small sleep in between so that the
  queue size drops slow enough for the other program to see it.

  Not much error checking here.

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
#include <sys/ioctl.h>

int main ()
{
    int  i;
    char ch = 'x';
    char stop = 'z';
    for ( i = 0; i < 100; i++ ) {
        ioctl(0, TIOCSTI, &ch);
        usleep(50000);
    }
    ioctl(0, TIOCSTI, &stop);
    while ( read(0 , &ch, 1) > 0 && ch != stop ) {
        usleep(50000); /* Delay a bit so ttywatch can see queue size drop. */
    }
    return 0;
}
