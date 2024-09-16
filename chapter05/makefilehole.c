/*****************************************************************************
  Title          : makefilehole.c
  Author         : Stewart Weiss
  Created on     : June 19, 2023
  Description    : Makes a file with a large hole
  Purpose        : Shows how lseek() can seek past end of file and we
                   can write past the end also.
  Usage          : makefilehole <filename>
  Build with     : gcc -I../include -L../lib -o makefilehole   makefilehole.c -lspl

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

#define _GNU_SOURCE
#include "common_hdrs.h"

#define MESSAGE_SIZE   512
#define BUFFER_SIZE     10

int main(int argc, char *argv[])
{
    int     fd;
    char    buffer[BUFFER_SIZE];
    char    message[MESSAGE_SIZE];

    if ( 2 > argc ) {
        sprintf(message, " %s <file-to-create>\n", basename(argv[0]));
        usage_error(message);
    }

/* Create a new file named file_with_hole in the pwd. */
    if ((fd = open(argv[1], O_WRONLY|O_CREAT|O_EXCL, 0644)) < 0) {
        sprintf(message, "unable to open file %s for writing", argv[1]);
        fatal_error(errno, message);
    }

    /* Fill buffer with a small string. */
    strncpy(buffer, "0123456789",BUFFER_SIZE);

    /* Write the small string at the beginning of the file. */
    if (write(fd, buffer, BUFFER_SIZE) != BUFFER_SIZE)
        fatal_error(errno, "write");

    /* Seek 1,000,000 bytes past the end of the file. */
    if (lseek(fd, 1000000, SEEK_END) == -1)
        fatal_error(errno, "lseek");

    /* Write the small string at the new file offset. */
    if (write(fd, buffer, BUFFER_SIZE) != BUFFER_SIZE)
        fatal_error(errno, "write");

    /* Close the file. */
    if ( close(fd) == -1 ) {
        sprintf(message, "error closing file %s\n", argv[1]);
        fatal_error(errno, message);
      }

    /* We now have a large file with a big hole. */
    exit(EXIT_SUCCESS);
}
