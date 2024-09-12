/*****************************************************************************
  Title          : curses_demo1.c
  Author         : Stewart Weiss
  Created on     : August 19, 2024
  Description    : Displays a string centered on the screen
  Purpose        : Show basic curses functions
                   It also shows how to "park" the cursor.
  Usage          : curses_demo1
  Build with     : gcc -Wall -I../include -L../lib -o curses_demo1 \
                      curses_demo1.c -lncurses =lspl

*****************************************************************************/
#include "common_hdrs.h"
#include  <curses.h>

int main(int argc, char* argv[])
{
    char   epoch[] = "January 1, 1970, the start of the UNIX Epoch";

    initscr();                              /* Initialize ncurses.          */
    move(LINES/2, (COLS-strlen(epoch))/2 );  /* Move to position so string is
                                                centered on screen.         */
    addstr( epoch );                         /* Write the string.           */
    move(LINES-1,0);     /* Park the cursor at bottom and display a prompt. */
    addstr("Type any char to quit:");
    refresh();

    /* Wait for user to type a character so that the screen doesn't clear.  */
    getch();
    endwin();  /* End curses session. */
    return 0;
}
