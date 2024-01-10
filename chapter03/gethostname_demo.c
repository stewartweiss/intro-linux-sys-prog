/*****************************************************************************
  Title          : gethostname_demo.c
  Author         : Stewart Weiss
  Created on     : December 10, 2022
  Description    : Produces an error message after call to gethostname()
  Purpose        : Show how to handle errors from a system call
  Usage          : gethostname_demo
  Build with     : gcc -o gethostname_demo gethostname_demo.c

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

#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main()
{
    char  name[4];   /* declare string to hold returned value */
    size_t len = 3;  /* purposely too small so error is revealed */
    int   returnvalue;

    returnvalue =  gethostname(name, len); /* make the call */
    if ( -1 == returnvalue ) {
        switch ( errno ) {
        case EFAULT:
            printf("A bad address was passed for the string name\n"); break;
        case EINVAL:
            printf("The length argument was negative.\n"); break;
        case ENAMETOOLONG:
            printf("The hostname is too long for the allocated array.\n");
        }
    }
    else
        printf("%s\n", name);
}

