/******************************************************************************
  Title          : gethost.c
  Author         : Stewart Weiss
  Created on     : January 17, 2023
  Description    : Produces an error message after call to gethostname()
  Purpose        : Show how to handle errors from a system call
  Usage          : gethost
  Build with     : gcc -o gethost gethost.c
  Modifications  :


********************************************************************************
 * Copyright (C) 2023 - Stewart Weiss
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.


*******************************************************************************/

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

