/*****************************************************************************
  Title          : wintest.c
  Author         : Stewart Weiss
  Created on     : July 28, 2024
  Description    : Displays a
  Purpose        : To show how
  Usage          :
  Build with     : gcc -Wall -I../include -L../lib -o wintest \
                      wintest.c  -lspl -lncurses


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
#include <curses.h>
#include <termios.h>


int main(int argc, char *argv[])
{
    WINDOW *my_win;
    int r, c;
    FILE *fp = fopen("out", "w");
    if ( NULL == fp ) return 1;

    initscr();                            /* Start curses mode.             */
    cbreak();                             /* Line buffering disabled.       */
    keypad(stdscr, TRUE);                 /* Enable  the keypad.            */
    mvhline(3,0, ACS_HLINE, COLS);        /* Draw horizontal line in row 3. */
    refresh();
    my_win = newwin(LINES-4, COLS, 4, 0); /* Create a new window.           */
    move(1,0);
    addstr("This is the part of stdscr not obscured by the new window.");
    getmaxyx(my_win, r, c);  /* Get dimensions of new window.               */
    wmove(my_win, r/2, 0);   /* Move to center row, column 0 of new window. */
    waddstr(my_win, "This is the new window, on top of stdscr.");
    refresh();
    wrefresh(my_win);
    putwin(my_win,fp);  /* Save window contents to file named "out" in cwd. */
    getch();            /* Wait for any character to be entered.            */
    endwin();           /* End curses mode.                                 */
    return 0;
}
