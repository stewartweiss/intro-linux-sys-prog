/*****************************************************************************
  Title          : fakeinput_demo.c
  Author         : Stewart Weiss
  Created on     : Jul 31, 2024
  Description    : Puts characters into terminal input queue.
  Purpose        : To show the risks of a dangerous ioctl call.
  Usage          : fakeinput_demo
  Build with     : gcc -Wall -g -o fakeinput_demo fakeinput_demo.c  \
                   -I../include -L../lib -lspl

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
#include <sys/termios.h>

int main (int argc, char* argv[])
{
    char word[]= "/usr/bin/echo 'This is a dangerous thing to do!'";
    char nl = '\n';
    struct termios tt;

    if ( tcgetattr(0,&tt) == -1 )
        fatal_error(errno, "tcgettattr");
    tt.c_lflag &= ~ECHO ;
    if ( tcsetattr(0,TCSANOW, &tt) == -1 )
        fatal_error(errno, "tcsettattr");

    for ( int i = 0; i < strlen(word); i++ )
      if ( -1 == ioctl(STDIN_FILENO, TIOCSTI, &word[i]) )
            printf("Cannot write characters into terminal input queue\n");
    sleep(2);
    ioctl(STDIN_FILENO, TIOCSTI, &nl);
    tt.c_lflag |=  ECHO ;
    if ( tcsetattr(0,TCSANOW, &tt) == -1 )
        fatal_error(errno, "tcsettattr");
    return 0;
}
