/*****************************************************************************
  Title          : showallusers.c
  Author         : Stewart Weiss
  Created on     :
  Description    : An implementation of the id command
  Purpose        : To demonstrate  ...
  Usage          : showallusers
  Build with     : gcc -o showusers   showusers.c  -lspl
  Modifications  :

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


*****************************************************************************/

#define _GNU_SOURCE
#include "common_hdrs.h"
#include <pwd.h>

int main( int argc, char* argv[])
{
    /* Declare a passwd structure to store returned record. */
    struct passwd  *psswd_struct;

    /* Initialize the iterator. */
    setpwent();

    /* Set errno to zero to detect errors by getpwent(). */
    errno = 0;

    /* Repeatedly call getpwent() until it returns NULL. */
    while ( (psswd_struct  = getpwent()) != NULL) {
        /* If not NULL, print the pw_name member of the struct. */
        printf("%s\n", psswd_struct->pw_name);
        errno = 0;
    }

    /* Check for an error other than a "no such file" error,
      which is error code ENOENT */
    if ( errno != 0 && errno != ENOENT ) {
        fatal_error(errno, "getpwent");
    }

    /* Close the passwd database. */
    endpwent();
    return 0;
}
