
#include "common_hdrs.h"
#include <curses.h>


int main()
{
    printf("The installed version of ncurses is %s\n",
    curses_version());
    return 0;
}
