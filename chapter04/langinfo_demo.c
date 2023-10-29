
/****************************************************************************
  Title          : langinfo_demo.c
  Author         : Stewart Weiss
  Created on     : March 13, 2023
  Description    : Displays names of days of the week in current locale
  Purpose        : To show how to use nl_langinfo()
  Usage          : LC_TIME=<locale-name> langinfo_demo
  Build with     : gcc -Wall -L../lib langinfo_demo.c -o langinfo_demo -lspl
  Modifications  :

  Notes
  This is intended to display the names of the days of the week in any
  supported locale. Change the LC_TIME category on the command line before
  the program invocation.

*****************************************************************************

Copyright (C) 2023 - Stewart Weiss

This code is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

****************************************************************************/

#define _GNU_SOURCE
#include <langinfo.h>
#include "common_hdrs.h"

int  main(int argc, char *argv[])
{
    char*  mylocale;
    if ( (mylocale = setlocale(LC_TIME, "") ) == NULL )
        fatal_error( LOCALE_ERROR,
           "setlocale() could not set the given locale");

    printf("The current locale is %s\n", mylocale);

    /* DAY_1 is a keyword defined in <langinfo.h>
       It is used to retrieve the name of the first day of the week
       in the current locale. The second day is DAY_2, and so on.
       Because they are consecutive integers, we can increment to
       advance through them. */
    int dayofweek = DAY_1;
    while ( dayofweek < DAY_1+7 ) {
        printf("%s\n", nl_langinfo(dayofweek));
        dayofweek++;
        /* This prints DAY_1, DAY_2, ..., DAY_7 */
    }
    return 0;
}

