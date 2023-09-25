/****************************************************************************
  Title          : newlocale_demo.c
  Author         : Stewart Weiss
  Created on     : March 13, 2023
  Description    : Combines two locale categories into single mixed locale
  Purpose        : To show how to use newlocale() to customize a locale
  Usage          : LC_TIME=<locale-name> langinfo_demo
  Build with     : gcc -Wall -L../lib newlocale_demo.c -o newlocale_demo -lspl
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
#define _XOPEN_SOURCE 700
#include <locale.h>
#include "../include/common_hdrs.h"
#include "../include/common_defs.h"

#define TESTNUMBER  123456789.12

int  main(int argc, char *argv[])
{
    time_t t;                  /* To store current time          */
    struct tm *tm;             /* To store broken-down time      */
    char buf[100];             /* To store formatted time string */
    char  err_msg[STRING_MAX]; /* For error messages             */
    locale_t loc, newloc;      /* Temporary locale objects       */

    if (argc < 2) {
        sprintf(err_msg, "Usage: %s locale1 [locale2]\n", argv[0]);
        usage_error( err_msg);
    }

    /* Create a new locale object, using the LC_NUMERIC settings
       from the locale specified in argv[1] */
    loc = newlocale(LC_NUMERIC_MASK, argv[1], (locale_t) 0);
    if (loc == (locale_t) 0)
        fatal_error(EXIT_FAILURE, "newlocale");

    /* Now we use the LC_TIME values of the second command-line
       argument to modify the locale object we just created.
       The effect is that it has the numeric settings of the first
       locale and the time settings of the second.
       We assign the returned locale to a new locale object so
       that we don't overwrite loc, in case something goes wrong. */
    if (argc > 2) {
        newloc = newlocale(LC_TIME_MASK, argv[2], loc);
        if (newloc == (locale_t) 0)
            fatal_error(EXIT_FAILURE, "newlocale");
        loc = newloc;
    }
    /* Use the newly created locale  */
    uselocale(loc);

    /* Test effect of LC_NUMERIC */
    printf("%'8.2f\n", TESTNUMBER);

    /* Test effect of LC_TIME */
    t = time(NULL);
    if ( (tm = localtime(&t)) == NULL)
        fatal_error(EXIT_FAILURE, "localtime");

    /* Call strftime using the %c format for locale default */
    if ( 0 == strftime(buf, sizeof(buf), "%c", tm) )
        fatal_error(EXIT_FAILURE, "strftime");
    printf("%s\n", buf);

    /* Free the loc locale object */
    uselocale(LC_GLOBAL_LOCALE);    /* So loc is no longer in use */
    freelocale(loc);                /* release storage for loc */
    return 0;
}
