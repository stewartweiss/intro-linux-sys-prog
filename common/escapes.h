/*****************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.lgplv3 for details.               *
*****************************************************************************/

#ifndef ESCAPES_H
#define ESCAPES_H

#include <termios.h>

char CLEAR_SCREEN  [] = "\033[2J";
char CLEAR_ALL     [] = "\033[3J";
char CLEAR_LINE    [] = "\033[1A\033[2K\033[G";
char CLEAR_ABOVE   [] = "\033[1J";
char CLEAR_DOWN    [] = "\033[0J";
char INVERSE_START [] = "\033[7m";
char INVERSE_END   [] = "\033[27m";
char PROMPT        [] = "prompt: ";
char CURSOR_HOME   [] = "\033[1;1H";
char CURSOR_PARK   [] = "\033[24;80H";
char CURSOR_UP_2   [] = "\033[2A";
char SCROLL_WINDOW [] = "\033M";
char ERASE_LINE    [] = "\033[2K";
char ERASE_RIGHT   [] = "\033[K";
char CURSOR_TO_LEFT[] = "\033[1G";

#endif // ESCAPES_H
