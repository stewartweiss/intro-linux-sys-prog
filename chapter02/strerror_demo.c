/*****************************************************************************
  Title          : strerror_demo.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Like gethostname_demo.c but produces an error message
  Purpose        : To demonstrate how to use handle argument shortage
  Usage          : strerror_demo
  Build with     : gcc -o strerror_demo strerror_demo.c

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
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    char  name[4];   /* declare string to hold returned value */
    size_t len = 3;  /* purposely too small so error is revealed */
    int   returnvalue;


    returnvalue =  gethostname(name, len); /* make the call */
    if ( -1 == returnvalue ) {
        printf("gethostname failed: %s\n", strerror(errno));
    }
    else
        printf("%s\n", name);
   
    return 0;
}
