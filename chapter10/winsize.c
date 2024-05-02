/*****************************************************************************
  Title          : winsize.c
  Author         : Stewart Weiss
  Created on     : March 10, 2008
  Description    : Displays window size in lines and rows (pixels usually fails)
  Purpose        : To demonstrate the ioctl() call
  Usage          : winsize
  Build with     : gcc -Wall -g -o winsize winsize.c

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
#include <termios.h>


#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

void get_winsize(int fd, unsigned short *rows, unsigned short *cols )
{
    struct winsize size;

    if (ioctl(fd, TIOCGWINSZ,  &size) < 0) {
        perror("TIOCGWINSZ error");
        return;
    }
    *rows = size.ws_row;
    *cols = size.ws_col;
}


/*****************************************************************************
                               Main Program
*****************************************************************************/

int main(int argc, char* argv[])
{
    unsigned short int rows, cols;

    if (isatty(STDIN_FILENO) == 0) {
        fprintf(stderr, "Not a terminal\n");
        exit(1);
    }
    get_winsize(STDIN_FILENO, &rows, &cols);   /* print  size */
    if ( rows > 0 )
        printf("%d rows, %d columns\n", rows, cols);
    return 0;
}

