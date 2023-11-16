/******************************************************************************
  Title          : printargs.c
  Author         : Stewart Weiss
  Created on     : January  9, 2023
  Description    : Shows how to access command-line arguments
  Purpose        : 
  Usage          : printargs <list of words>
  Build with     : gcc -o printargs printargs.c
 
******************************************************************************/


/******************************************************************************
 * Copyright (C) 2019 - Stewart Weiss
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
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Arguments found: ");
    for ( int i = 1; i < argc ; i++ ){
         printf("(%d) %s ", i, argv[i]);
    }

    printf("\n");
    return 0;
}
