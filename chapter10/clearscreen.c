/******************************************************************************
  Title          : escapes.c
  Author         : Stewart Weiss
  Created on     : January  2, 2011
  Description    : Demonstrates use of ANSI escape sequences
  Purpose        : To show the effects of various ANSI escape sequences
  Usage          : vt100ctrl
  Build with     : gcc -o escapes escapes.c
  Modifications  : January 23, 2012
                   Comments revised and a few extra sequences added.



******************************************************************************
 * Copyright (C) 2019 - Stewart Weiss
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.


******************************************************************************/
#define _GNU_SOURCE
#include "common_hdrs.h"
#include <termios.h>

#define CLEAR_SCREEN    "\033[2J"

int main (int argc, char *argv[])
{
    printf(  "%s", CLEAR_SCREEN);
    return (0);
}





