#include <ncurses.h>
#include <stdio.h>


WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main(int argc, char *argv[])
{
    WINDOW *my_win;
    int ch;
    int r, c;
    FILE *fp = fopen("out", "w");
    if ( NULL == fp ) return 1;

    initscr();            /* Start curses mode         */
    cbreak();            /* Line buffering disabled, Pass on
                     * everty thing to me         */
    keypad(stdscr, TRUE);
    mvhline(3,0, ACS_HLINE, COLS);
    refresh();
    my_win = newwin(LINES-4, COLS, 4, 0);
    move(1,0);
    addstr("This is the part of stdscr not obscured by the new window.");
    getmaxyx(my_win, r, c);
    wmove(my_win, r/2, 0);
    waddstr(my_win, "This is the new window, on top of stdscr.");
    refresh();
    wrefresh(my_win);
    putwin(my_win,fp);
    getch();
    endwin();            /* End curses mode          */
    return 0;
}
