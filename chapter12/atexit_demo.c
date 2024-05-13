/*****************************************************************************
  Title          : atexit_demo.c
  Author         : Stewart Weiss
  Created on     : April 15, 2024
  Description    : Displays messages from functions registered with atexit()
  Purpose        : To show how to register exit functions.
  Usage          : atexit_demo
  Build with     :

******************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/


#include "common_hdrs.h"

void lockingup(void)
{
    printf("Locking up. Goodbye.\n");
}

void mopper(void)
{
    printf("Mopping the floors.\n");
}

void sweeper(void)
{
    printf("Sweeping the floor and wiping down counters.\n");
}

void supervisor(void)
{
    printf("Time to clean up.\n");
}

int main(void)
{
    long max_exit_functions = sysconf(_SC_ATEXIT_MAX);

    printf("The maximum number of exit functions is %ld\n",
            max_exit_functions);
    if ((atexit(lockingup)) != 0)
        fatal_error(errno, "cannot set exit function\n");
    if ((atexit(mopper)) != 0)
        fatal_error(errno, "cannot set exit function\n");
    if ((atexit(sweeper)) != 0)
        fatal_error(errno, "cannot set exit function\n");
    if ((atexit(supervisor)) != 0)
        fatal_error(errno, "cannot set exit function\n");
    return EXIT_SUCCESS; /* return invokes exit() */
}

