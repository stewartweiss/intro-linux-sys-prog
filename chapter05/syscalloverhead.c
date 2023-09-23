/*****************************************************************************
  Title          : syscalloverhead.c
  Author         : Stewart Weiss
  Created on     : August 1, 2023
  Description    : Call uname sufficiently many times to see cpu usage
  Purpose        : To measure CPU usage of library call
  Usage          : time -p  syscalloverhead
  Build with     : gcc -Wall -g  -o syscalloverhead  syscalloverhead.c

******************************************************************************
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

*****************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h>

int main(int argc, char* argv[])
{
    int i;
    struct utsname data;

    for ( i = 1; i < 100000000; i++ )
        uname(&data);
    return 0;
}
