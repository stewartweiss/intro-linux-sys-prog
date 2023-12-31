/******************************************************************************
  Title          : showtid.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Displays the thread id of the calling process
  Purpose        : To demonstrate how to use the syscall() function
  Usage          : showtid
  Build with     : gcc -o showtid showtid.c
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


*******************************************************************************/

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Thread id %ld\n", syscall(SYS_gettid));
    /* could also pass __NR_gettid */
    return 0;
}
