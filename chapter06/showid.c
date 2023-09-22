/******************************************************************************
  Title          : showid.c
  Author         : Stewart Weiss
  Created on     : January 27, 2012
  Description    : An implementation of the id command
  Purpose        : To demonstrate how to access identity-related kernel
                   databases using various parts of the API related to identity
                   including getuid(), getgid(), getpwuid(), getgrgid(), and
                   getgroups().
  Usage          : showid
  Build with     : gcc -o showid   showid.c
  Modifications  : August 12, 2023
                   Clarified documentation

******************************************************************************
 * Copyright (C) 2023 - Stewart Weiss
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>


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
