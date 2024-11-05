/*****************************************************************************
  Title          : test_get_long.c
  Author         : Stewart Weiss
  Created on     : Feb. 20, 2023
  Description    : Shows the effect of get_long()'s flags
  Purpose        : To test the get_long() function
  Usage          : test_get_long  <str>
  Build with     : gcc -g -Wall -o test_get_long -I../include -L../lib \
                      test_get_long.c -lspl

******************************************************************************
* Copyright (C) 2024 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include "common_hdrs.h"
#include "get_nums.h"

void printret(int ret)
{
    switch ( ret ){
    case VALID_NUMBER          : printf("VALID_NUMBER "); break;
    case FATAL_ERROR           : printf("FATAL_ERROR ");break;
    case TRAILING_CHARS_FOUND  : printf("TRAILING_CHARS_FOUND ");break;
    case OUT_OF_RANGE          : printf("OUT_OF_RANGE ");break;
    case NO_DIGITS_FOUND       : printf("NO_DIGITS_FOUND ");break;
    case NEG_NUM_FOUND         : printf("NEG_NUM_FOUND ");break;
    }

}

int  main(int argc, char *argv[])
{
    int res = 0;
    int flags = 0;
    long number;
    char msg[1024];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s str \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    flags = NO_TRAILING;
    printf("flags = %d\n ", flags);
    res =  get_long(argv[1], flags, &number, msg );

    if ( VALID_NUMBER == res ) {
        printret(res);
        printf("The value is %ld\n", number);
        }
    else {
        printret(res);printf("%s", msg);
    }

    flags = ONLY_DIGITS;
    printf("flags = %d\n", flags);
    res =  get_long(argv[1], flags, &number, msg );

    if ( VALID_NUMBER == res ){
        printret(res);
        printf("The value is %ld\n", number);
        }
    else {
        printret(res);printf("%s", msg);
    }

    flags = NO_TRAILING | ONLY_DIGITS;
    printf("flags = %d\n ", flags);
    res =  get_long(argv[1], flags, &number, msg );


    if ( VALID_NUMBER == res ){
        printret(res);
        printf("The value is %ld\n", number);
        }
    else {
        printret(res);printf("%s", msg);
    }

    flags = 0;
    printf("flags = %d\n", flags);
    res =  get_long(argv[1], flags, &number, msg );


    if ( VALID_NUMBER == res ){
        printret(res);
        printf("The value is %ld\n", number);
        }
    else {
        printret(res);printf("%s", msg);
    }

    flags = NON_NEG_ONLY;
    printf("flags = %d\n ", flags);
    res =  get_long(argv[1], flags, &number, msg );


    if ( VALID_NUMBER == res ){
        printret(res);
        printf("The value is %ld\n", number);
        }
    else {
        printret(res);printf("%s", msg);
    }

    return 0;

}
