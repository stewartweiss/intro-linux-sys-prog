/*****************************************************************************
  Title          : fakelogin.c
  Author         : Stewart Weiss
  Created on     : July 20, 2024
  Description    : Display login and get username and password
  Purpose        : Demonstrates use of tcgetattr and tcsetattr to turn
                   off echo and turn it back on.
  Usage          : fakelogin
  Build with     :

  Notes          : This program does not simulate what the real login process
                   does. It is the getty() process that gets the
                   username and passes that name to login().
                   Then, login() displays the password prompt, gets the
                   user's entered password, and uses the password database
                   to authenticate the user.

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
#include   <termios.h>

const  char loginstr[]  = "login: ";
const  char passwdstr[] = "password: ";

int main(int argc, char* argv[] )
{
    struct termios tt;
    char username[33];
    char passwd[33];
    int  ttyfd, n;

    if ( -1 == (ttyfd = open("/dev/tty", O_RDWR)) )
        fatal_error(errno, "open");

    memset(username, 0, 33);    /* Make sure buffers are filled with NULLs. */
    memset(passwd,   0, 33);

    if ( -1 == write(ttyfd, loginstr, strlen(loginstr)))
        fatal_error(errno, "write");
    if ( -1 == ( n = read(ttyfd, username, 32)))  /* Get user's username.   */
        fatal_error(errno, "read");
    if (username[n-1] == '\n' )
        username[n-1] = '\0';                   /* Get rid of \n at end.    */
    if ( -1 == tcflush(ttyfd, TCIFLUSH))
        fatal_error(errno, "tcflush");

    /* Now turn off echo.  */
    if (-1 == tcgetattr(ttyfd, &tt))         /* Get current terminal state. */
        fatal_error(errno, "tcgetattr");

    tt.c_lflag &= ~ECHO ;                    /* Turn off echo bit.          */
    if ( -1 == tcsetattr(ttyfd,TCSANOW,&tt))   /* Use this new structure.  */
        fatal_error(errno, "tcsetattr");
    if ( -1 == write(ttyfd, passwdstr, strlen(passwdstr)))
        fatal_error(errno, "write");
    if ( -1 == (n =read( ttyfd, passwd, 32))) /* Get user's hidden typing.  */
        fatal_error(errno, "read");
    if (passwd[n-1] == '\n' )
        passwd[n-1] = '\0';     /* Get rid of \n at end.    */
    if ( -1 == tcflush(ttyfd, TCIFLUSH))
        fatal_error(errno, "tcflush");
    tt.c_lflag |= ECHO ;                     /* Turn echo on.               */
    if ( -1 == tcsetattr(ttyfd,TCSAFLUSH,&tt))   /* Restore  settings.     */
        fatal_error(errno, "tcsetattr");
    printf("\nUser %s entered %s as a password.\n", username, passwd);
    return 0;
}
