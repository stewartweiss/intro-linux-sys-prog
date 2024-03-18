/*****************************************************************************
  Title          : clearscreen.c
  Author         : Stewart Weiss
  Created on     : January  2, 2011
  Description    : Clears the terminal window using ANSI escape
  Purpose        : To show how to use an ANSI escape sequence
  Usage          : clearscreen
  Build with     : gcc -o clearscreen -I../include clearscreen.c

******************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#define _GNU_SOURCE
#include "common_hdrs.h"
#include <termios.h>

#define CLEAR_SCREEN    "\033[2J"

int main (int argc, char *argv[])
{
    printf(  "%s", CLEAR_SCREEN);
    return (0);
}
