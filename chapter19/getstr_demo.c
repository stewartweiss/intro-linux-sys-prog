/*****************************************************************************
  Title          : getstr_demo.c
  Author         : Stewart Weiss
  Created on     : August 19, 2024
  Description    : Gets a string from user and prints it on screen.
  Purpose        : Shows use of getstr() in cbreak mode.
  Usage          : getstr_demo
  Build with     : gcc -Wall -I../include -L../lib -o getstr_demo \
                      getstr_demo.c -lncurses -lspl

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
#include  <curses.h>

int main(int argc, char* argv[])
{
    char str[32];

    initscr();
    cbreak();     /* Put into cbreak mode. */
    mvaddstr(0,0,"Type up to 31 characters and press ENTER:");
    getstr(str);
    mvaddstr(1,0, "You entered: ");
    addstr(str);
    mvaddstr(2,0,"Type any character to quit.");
    getch();      /* Pause until user enters anything. */
    endwin();
    return 0;
}
