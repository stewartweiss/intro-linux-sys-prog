/*****************************************************************************
  Title          : showallusers.c
  Author         : Stewart Weiss
  Created on     :
  Description    : Prints the usernames of all users in the passwd database
  Purpose        : To demonstrate  how to use getpwent()
  Usage          : showallusers
  Build with     : gcc -I../include -L../lib -o showallusers  showallusers.c \
                    -lspl

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
