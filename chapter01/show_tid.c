/******************************************************************************
  Title          : show_tid.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Shows the thread id of the calling process
  Purpose        : To demonstrate how a thread can get its thread id
  Usage          : show_tid
  Build with     : gcc -o show_tid show_tid.c
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


#define _GNU_SOURCE   /* This is a macro that activates GNU source code in the
                         subsequently included header files. gettid() is not
                         required by SUSv3; it is a GNU extension.           */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("I am a thread with thread-id %d\n", gettid());
}
