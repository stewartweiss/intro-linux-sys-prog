/*****************************************************************************
  Title          : risky_getenv.c
  Author         : Stewart Weiss
  Created on     : October 18, 2023
  Description/   : A bad way to use the getenv() function
  Purpose        : To show how NOT to call getenv()
  Usage          : risky_getenv 
  Build with     : gcc -Wall -g -I../include -L../lib -o risky_getenv \
                   risky_getenv.c -lspl

  Notes:
  The getenv() function may be implemented in such a way that it
  allocates a static pointer to the environment string that it finds.
  The fact that it's static implies that there's just a single copy
  of it in the process's address space, and it is not in the process
  stack. 
  If it's implemented this way then each successive call to getenv() 
  within a single process will overwrite this pointer. The program
  needs to copy the pointer before calling it again if it does not use
  its value right away.

******************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>

int main()
{
    char *home, *user;
    home  = getenv("HOME"); /* home points to pathname to home directory. */
    if ( NULL != home ) {
        user = getenv("USER");  /* user points to pathname to username.   */
        /* If the internal pointer is static, home and user are copies of
           the same pointer, and the next printf() will print the same
           string for each. */
        if ( NULL != user )
            printf("USER=%s and HOME=%s\n", user, home);
    }
    return 0;
}
