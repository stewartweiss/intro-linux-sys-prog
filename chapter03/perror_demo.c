/*****************************************************************************
  Title          : perror_demo.c
  Author         : Stewart Weiss
  Created on     : January 18, 2023
  Description    : Like gethostname_demo.c but produces an error message
  Purpose        : Show how to use perror() for handling system call errors
  Usage          : perror_demo
  Build with     : gcc -o perror_demo perror_demo.c

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

int main(int argc, char* argv[] )
{
    char  name[4];   /* declare string to hold returned value */
    size_t len = 3;  /* purposely too small so error is revealed */
    int   returnvalue;


    returnvalue =  gethostname(name, len); /* make the call */
    if ( -1 == returnvalue ) {
        perror("gethostname:");
    }
    else
        printf("%s\n", name);

    return 0;
}
