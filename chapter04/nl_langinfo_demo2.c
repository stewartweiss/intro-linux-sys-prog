/*****************************************************************************
  Title          : nl_langinfo_demo2.c
  Author         : Stewart Weiss
  Created on     : March 13, 2023
  Description    : Displays format used for dates in current locale
  Purpose        : To show how to use nl_langinfo()
  Usage          : LC_TIME=<locale-name> langinfo_demo
  Build with     : gcc -Wall -I../include -L../lib langinfo_demo.c \
                    -o langinfo_demo -lspl


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
#include <langinfo.h>
#include "common_hdrs.h"

int  main(int argc, char *argv[])
{
    char*  mylocale;
    if ( (mylocale = setlocale(LC_TIME, "") ) == NULL )
        fatal_error( LOCALE_ERROR,
           "setlocale() could not set the given locale");

    printf("The  default date format is %s\n", nl_langinfo(D_T_FMT));

    return 0;
}

