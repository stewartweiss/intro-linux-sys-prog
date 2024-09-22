/*****************************************************************************
  Title          : curses_demo1.c
  Author         : Stewart Weiss
  Created on     : August 19, 2024
  Description    : Displays a string centered on the screen
  Purpose        : Show basic curses functions
                   It also shows how to "park" the cursor.
  Usage          : curses_demo1
  Build with     : gcc -Wall -I../include -L../lib -o curses_demo1 \
                      curses_demo1.c -lncurses -lspl

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
    char   epoch[] = "January 1, 1970, the start of the UNIX Epoch";

    initscr();                              /* Initialize ncurses.          */
    move(LINES/2, (COLS-strlen(epoch))/2 ); /* Move to position at which
                                               string is centered on screen.*/
    addstr( epoch );                         /* Write the string.           */
    move(LINES-1,0);     /* Park the cursor at bottom and display a prompt. */
    addstr("Type any char to quit:");
    refresh();

    /* Wait for user to type a character so that the screen doesn't clear.  */
    getch();
    endwin();  /* End curses session. */
    return 0;
}
