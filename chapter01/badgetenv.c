/******************************************************************************
  Title          : bad_getenv_demo.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Will not compile
  Purpose        : To show an incorrect way to call getenv()
  Usage          : Cannot use this
  Build with     : gcc -o bad_getenv_demo bad_getenv_demo.c
  Modifications  :


******************************************************************************
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


******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char num_columns[200];
    num_columns = getenv("COLUMNS");  /* This cannot compile because num_columns
                                         is a const char* but getenv() allocates
                                         its own memory for the returned string
                                         and returns its address. */
    printf("This window has %s columns.\n", num_columns);
};
