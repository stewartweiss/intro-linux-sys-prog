/*****************************************************************************
  Title          : show_pid.c
  Author         : Stewart Weiss
  Created on     : December 11, 2022
  Description    : Shows the process id of the calling process
  Purpose        : To demonstrate a first system call
  Usage          : show_pid
  Build with     : gcc -o show_pid show_pid.c
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
#include <unistd.h>
int main()
{
    printf("I am the process with process-id %d\n", getpid());
}
