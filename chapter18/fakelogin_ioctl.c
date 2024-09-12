/*****************************************************************************
  Title          : login_ioctl.c
  Author         : Stewart Weiss
  Created on     : March 12, 2013
  Description    : Display login and get username and password
  Purpose        : Demonstrates use of ioctl to turn
                   off echo and turn it back on.
  Usage          : login_ioctl
  Build with     : gcc -Wall -g -o login_ioctl login_ioctl.c
  Modifications  :

  Notes          : This program does not simulate what the login process
                   really does. It is the getty() process that gets the
                   username, not login(), and passes that name to login().
                   Then, login() displays the password prompt, gets the
                   user's entered password, and uses the password database
                   to authenticate the user.

*****************************************************************************/

#include   <stdio.h>
#include   <unistd.h>
#include   <stdlib.h>
#include   <termios.h>
#include <sys/ioctl.h>
#include  "utils.h"


int main(int argc, char* argv[] )
{
    struct termios info, orig;
    char username[33];
    char passwd[33];
    FILE   *fp;
    int fd;

    /* get a FILE* to the control terminal --   don't assume stdin  */
    if ((fp = fopen(ctermid(NULL), "r+")) == NULL)
         return(1);
    fd = fileno(fp);

    printf("login: ");                   /* display message          */
    fgets(username, 32, stdin);          /* get user's typing        */

    /* Now turn off echo */
    if ( ioctl( fd , TCGETS, &info ) == -1 )
        die("ioctl", "1");
    orig = info;                         /* Save a copy of it */
    info.c_lflag &= ~ECHO ;              /* Turn off echo bit   */
    if ( ioctl(fd,TCSETS, &info) == -1 )
        die("ioctl","2");

    printf("password: ");
    fgets( passwd, 32, stdin);           /* Get user's non-echoed typing   */

    if ( ioctl( fd , TCSETS, &orig ) == -1 )
        die("ioctl", "1");

    printf("\n");                        /* Print a fake message      */
    printf("Last login: Tue Apr 31 21:29:54 2088 from the twilight zone.\n");
    return 0;
}
