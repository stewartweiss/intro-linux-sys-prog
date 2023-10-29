/******************************************************************************
  Title          : monetary_demo.c
  Author         : Stewart Weiss
  Created on     : February  22, 2023
  Description    : Displays monetary value in different locale currencies
  Purpose        : To show how to use strfmon()
  Usage          : monetary_demo <amount>
  Build with     : gcc -Wall -L../lib monetary_demo.c -o monetary_demo -lspl

 * Copyright (C) 2023 - Stewart Weiss
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#define _GNU_SOURCE
#include <langinfo.h>
#include <monetary.h>
#include "common_hdrs.h"


void  using_locale (char *new_locale, void (*func) (double), double argument)
{
    char *old_locale, *saved_locale;

    /* Get the name of the current locale. */
    old_locale = setlocale (LC_ALL, NULL);

    /* Copy the name so it won’t be clobbered by setlocale. */
    saved_locale = strdup (old_locale);
    if (saved_locale == NULL)
        fatal_error (-1,"could not create a new locale");

    /* Now change the locale. */
    setlocale (LC_ALL, new_locale);
    /* Call the function passed in. */
    (*func) (argument);

    /* Restore the original locale. */
    setlocale (LC_ALL, saved_locale);
    free (saved_locale);
}

void print_amount(double amount)
{
    char buffer[200];

    strfmon(buffer, sizeof(buffer), "local: %= #6n;\tinternational: %= #6i",
                   amount, amount);
    printf("%s\n", buffer);

}

int main( int argc, char* argv[])
{
    if ( argc < 2) {
        usage_error("missing argument");
    }

    setlocale(LC_ALL, "");
    double amount = atof(argv[1]);

    using_locale("de_DE.utf8", print_amount, amount);
    using_locale("ja_JP.utf8", print_amount, amount);
    using_locale("es_ES.utf8", print_amount, amount);
    using_locale("en_GB.utf8", print_amount, amount);
    using_locale("quz_PE.UTF-8", print_amount, amount);
    using_locale("sa_IN.UTF-8", print_amount, amount);
    using_locale("zh_CN.UTF-8", print_amount, amount);
    using_locale("zh_HK.UTF-8", print_amount, amount);
    using_locale("zh_SG.UTF-8", print_amount, amount);
    using_locale("zh_TW.UTF-8", print_amount, amount);
    return 0;
}

