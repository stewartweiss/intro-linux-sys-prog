/*****************************************************************************
  Title          : makelargefile.c
  Author         : Stewart Weiss
  Created on     : Dec 16, 2023
  Description    : Makes a file with a large hole
  Purpose        : Shows how lseek() can seek past end of file and we
                   can write past the end also.
  Usage          : makelargefile file size
  Build with     : gcc -I../include -L../lib -o makelargefile \
                      makelargefile.c  -lspl
  Note:
  After creating the file check its size and block usage with
  ls -ls file_with_hole
  You will see that it is 131082 bytes but does not use more than a few blocks.


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
    int     errcode;
    long    size;

    if ( argc < 3 )
        usage_error("usage: this  file-to-create  size\n");

    if ( VALID_NUMBER != ( errcode = get_long(argv[2],
                      NON_NEG_ONLY, &size, message ) ) )
        fatal_error( errcode, "get_long");

    /* Create a new file named file_with_hole in the pwd. */
    if ((fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0) {
        sprintf(message, "Unable to open file %s for writing", argv[1]);
        fatal_error(errno, message);
    }

    /* Fill buffer with a small string. */
    strncpy(buffer, "start", strlen("start")+1);

    /* Write the small string at the beginning of the file. */
    if (write(fd, buffer, strlen(buffer)) != strlen(buffer))
        fatal_error(errno, "write");

    /* Seek size-5 bytes past the start of the file. */
    if (lseek(fd, size - strlen("start") , SEEK_SET) == -1)
        fatal_error(errno, "lseek");

    /* Write the small string at the new file offset. */
    if (write(fd, buffer, strlen(buffer)) != strlen(buffer))
        fatal_error(errno, "write");

    /* Close the file. */
    if ( close(fd) == -1 ) {
        sprintf(message, "error closing file %s\n", argv[1]);
        fatal_error(errno, message);
      }

    /* We now have a large file with a big hole. */
    exit(EXIT_SUCCESS);
}
