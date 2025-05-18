/*****************************************************************************
  Title          : tiled_windows.c
  Author         : Stewart Weiss
  Created on     : August 23, 2024
  Description    : Creates two horizontally-tiled windows
  Purpose        : To show how to tile a terminal screen in curses.
  Usage          : tiled_windows.c
  Build with     : gcc -Wall -I../include -L../lib -o tiled_windows \
                      tiled_windows.c  -lspl -lncurses -lm


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
#include <ncurses.h>

const char info_bar[] = "A menu and status information could be here.\n"
                        "Type 's' to save the content area, or 'q' to quit:";

int main(int argc, char *argv[])
{
    WINDOW *content_win;      /* The content area     */
    WINDOW *info_win;         /* The information area */
    FILE   *fp;               /* File pointer for saving content window     */
    char   ch;                /* To store user input  */
    int    infobar_length  = strlen(info_bar);  /* Length of fixed message  */

    if ( NULL == (fp = fopen("./saved_content.crs", "w")))
        fatal_error(errno, "fopen");

    initscr();   /* Initialize curses.    */
    cbreak();    /* Put into cbreak mode. */
    noecho();    /* Turn off echo.        */

    /* Create a content window in the lower LINES-3 rows of the screen.     */
    if ( NULL == (content_win = newwin(LINES-3, COLS, 3, 0)) ) {
        endwin();
        fatal_error(-1, "Could not create first window.");
    }
    mvwaddstr(content_win,1, 0, "This is a content area.");
    wrefresh(content_win);                   /* Refresh this window.        */

    /* Create an information window in the top 3 rows of the screen.        */
    if ( NULL == (info_win   = newwin(3, COLS, 0, 0)) ) {
        endwin();
        fatal_error(-1, "Could not create second window.");
    }
    /* Fill the third row with a horizontal line. */
    mvwhline(info_win, 2,0, ACS_HLINE, COLS);
    mvwaddstr(info_win, 0, 0, info_bar);     /* Add info to top window.     */
    wmove( info_win,1, infobar_length);      /* Move  cursor to top window. */
    wrefresh(info_win);                      /* Refresh this window.        */

    /* Wait for input before terminating. */
    while ( (ch = wgetch(info_win) )!= 'q' ) {
        if ( ch == 's' )  /* Then save the window into a file. */
            putwin(content_win, fp);
    }
    endwin();
    return 0;
}
