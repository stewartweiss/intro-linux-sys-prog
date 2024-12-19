/*****************************************************************************
  Title          : fakelogin_ioctl.c
  Author         : Stewart Weiss
  Created on     : July 21, 2024
  Description    : Display login and get username and password
  Purpose        : Demonstrates use of ioctl to turn
                   off echo and turn it back on.
  Usage          : fakelogin_ioctl
  Build with     : gcc -Wall -L../lib -I../include -o fakelogin_ioctl \
                     fakelogin_ioctl.c -lspl

  Notes          : This program does not simulate what the login process
                   really does. It is the getty() process that gets the
                   username, not login(), and passes that name to login().
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
#include   <termios.h>
#ifndef TIOCGWINSZ
    #include <sys/ioctl.h>
#endif

int main(int argc, char *argv[] )
{
    struct termios info, orig;
    char username[33];
    char passwd[33];
    FILE   *fp;
    int fd;

    /* Get a FILE* to the control terminal --   don't assume stdin.         */
    if ((fp = fopen(ctermid(NULL), "r+")) == NULL)
         return(1);
    fd = fileno(fp);

    printf("login: ");                    /* Display message.               */
    fgets(username, 32, stdin);           /* Get user's typing.             */

    /* Now turn off echo */
    if ( ioctl( fd , TCGETS, &info ) == -1 )
        fatal_error(errno, "ioctl");
    orig = info;                          /* Save a copy of it.             */
    info.c_lflag &= ~ECHO ;               /* Turn off echo bit.             */
    if ( ioctl(fd,TCSETS, &info) == -1 )
        fatal_error(errno, "ioctl");

    printf("password: ");
    fgets( passwd, 32, stdin);             /* Get user's non-echoed typing. */

    if ( ioctl( fd , TCSETS, &orig ) == -1 )
        fatal_error(errno, "ioctl");

    printf("\n");                          /* Print a fake message.         */
    printf("Last login: Tue Apr 31 21:29:54 2088 from the twilight zone.\n");
    return 0;
}
