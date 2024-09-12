/*****************************************************************************
  Title          : toggle_echo.c
  Author         : Stewart Weiss
  Created on     : July 20, 2024
  Description    : Disables echo if on, enables if off
  Purpose        : Shows how to set the state of a flag in the termios struct
  Usage          : toggle_echo
  Build with     : gcc -Wall -g -o toggle_echo toggle_echo.c  -I../include \
                   -L../lib -lspl

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
#include  <termios.h>

int main(int argc, char *argv[])
{
    struct termios tty_attributes;              /* Local termios structure  */

    if (! isatty(STDIN_FILENO) )
        usage_error("Do not redirect standard input to this program.");

    if ( tcgetattr(0,&tty_attributes) == -1 )   /* Retrieve termios struct. */
        fatal_error(errno, "tcgettattr");

    if ( tty_attributes.c_lflag & ECHO )                  /* If enabled...  */
        tty_attributes.c_lflag &= ~ECHO ;                       /* Disable. */
    else                                                  /* Else           */
        tty_attributes.c_lflag |=  ECHO ;                       /*  Enable. */

    /* Replace termios with changed copy; use TCSANOW for immediate change. */
    if ( tcsetattr(0,TCSANOW, &tty_attributes) == -1 )
        fatal_error(errno, "tcsettattr");
    return 0;
}
