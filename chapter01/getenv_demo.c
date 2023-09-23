/*****************************************************************************
  Title          : getenv_demo.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Displays value of COLUMNS environment variable
  Purpose        : To show one way to get value of an environment variable
  Usage          : getenv_demo
  Build with     : gcc -o getenv_demo getenv_demo.c
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


*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char* num_columns = getenv("COLUMNS");
    printf("This window has %s columns.\n", num_columns);
};
