/*****************************************************************************
  Title          : watchtty.c
  Author         : Stewart Weiss
  Created on     : March 19, 2013
  Description    : Keeps a small log of the size of specified terminal queue
  Purpose        : Demonstrates use of ioctl TIOCINQ for getting size of
                   terminal input queue.
  Usage          : ioctl3 terminal-device
  Build with     : gcc -Wall -L../lib -I../include -o watchtty watchtty.c \
                      -lspl
  Notes          :
  This is designed to be used in conjunction with fillqueue.c, which adds
  characters from a terminal queue sufficiently slowly so that this can
  report the distinct sizes.


******************************************************************************
* Copyright (C) 2024 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/
#include "common_hdrs.h"
#include   <sys/ioctl.h>

int main(int argc, char *argv[] )
{
    int     count;
    int     fd;

    /* Try to open given terminal device file */
    if ((fd = open(argv[1], O_RDONLY)) == -1)
        usage_error("watchtty  <device-file>");

    while ( TRUE ) {
        if ( ioctl( fd, TIOCINQ, &count ) == -1 )
            fatal_error( errno, "ioctl TIOCINQ");
        printf("%d chars in queue\n", count);
        usleep(100000);  /* Delay to see changes */
    }
    return 0;
}
