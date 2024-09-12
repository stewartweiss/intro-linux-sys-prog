/*****************************************************************************
  Title          : getstr_demo.c
  Author         : Stewart Weiss
  Created on     : August 19, 2024
  Description    : Gets a strng from user and prints it on screen.
  Purpose        : Shows use of getstr() in cbreak mode.
  Usage          : getstr_demo
  Build with     : gcc -Wall -I../include -L../lib -o getstr_demo \
                      getstr_demo.c -lncurses =lspl

*****************************************************************************/
#include "common_hdrs.h"
#include  <curses.h>

int main(int argc, char* argv[])
{
    char str[32];

    initscr();
    cbreak();
    mvaddstr(0,0,"Type up to 31 characters and press ENTER:");
    getstr(str);
    mvaddstr(1,0, "You entered: ");
    addstr(str);
    mvaddstr(2,0,"Type any character to quit.");
    getch();
    endwin();
    return 0;
}
