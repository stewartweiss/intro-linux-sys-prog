/*****************************************************************************
  Title          : curses_version.c
  Author         : Stewart Weiss
  Created on     : August 3, 2024
  Description    : Prints version of curses library on screen.
  Purpose        : To check which curses is installed.
  Usage          : curses_version
  Build with     : gcc -Wall -I../include -L../lib -o curses_version \
                      curses_version.c -lncurses -lspl


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


int main()
{
    printf("The installed version of ncurses is %s\n",
    curses_version());
    return 0;
}
