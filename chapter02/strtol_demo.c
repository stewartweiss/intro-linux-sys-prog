/*****************************************************************************
  Title          : strtol_demo.c
  Author         : Stewart Weiss
  Created on     : January  2, 2023
  Description    : Converts its only argument to a long integer
  Purpose        : To demonstrate how to use strtol() for conversions
  Usage          : strtol_demo
  Build with     : gcc -o strtol_demo strtol_demo.c

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

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int  main(int argc, char *argv[])
{
    char *endptr;
    long val;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s str \n", argv[0]);
        exit(EXIT_FAILURE);
    }
    errno = 0;    /* To distinguish success/failure after call */
    val = strtol(argv[1], &endptr, 0);

    /* Check for various possible errors */
    if (errno != 0) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }
    /* errno == 0 */
    if (endptr == argv[1]) {
        /* the first invalid char is the first char of the string */
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }

    if (*endptr != '\0')
      /* there are non-number characters following the number.
         which we can call an error or not, depending. */
       printf("Characters following the number: \"%s\"\n", endptr);

    /* If we got here, strtol() successfully parsed a number */
    printf("strtol() returned %ld\n", val);
    exit(EXIT_SUCCESS);
}
