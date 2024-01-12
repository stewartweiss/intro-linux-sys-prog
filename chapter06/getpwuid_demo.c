/*****************************************************************************
  Title          : getpwuid_demo.c
  Author         : Stewart Weiss
  Created on     : January 27, 2012
  Description    : An implementation of a simple id command
  Purpose        : To demonstrate how to access identity-related kernel
                   databases using parts of the API related to identity
                   including getuid(), getgid(), getpwuid(), getgrgid(), and
                   getgroups().
  Usage          : getpwuid_demo
  Build with     : gcc -I../include -L../lib -o getpwuid_demo   \
                   getpwuid_demo.c -lspl

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

#include "common_hdrs.h"
#include <pwd.h>

int main( int argc, char* argv[])
{
    uid_t           userid;
    struct passwd  *psswd_struct;


    /* Get the real user id associated with the process
     * which is the same as that of the user who runs this command.  */
    userid        = getuid();

    /* To get the user name, we retrieve the password structure
     * from the real user id  using the following
     * function.                                                     */
    psswd_struct  = getpwuid(userid);

    /* Print out the user idw ith the name, in the same format as
     * the  id command:
     */
    printf( "uid=%d(%s)\n", userid, psswd_struct->pw_name);
    return 0;
}
