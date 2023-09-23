/******************************************************************************
  Title          : mk_filehole.c
  Author         : Stewart Weiss
  Created on     : June 19, 2023
  Description    : Makes a file with a large hole
  Purpose        : Shows how lseek() can seek past end of file and we
                   can write past the end also.
  Usage          : mk_filehole
  Build with     : gcc -o mk_filehole   mk_filehole.c
  Modifications  :

  Note:
  After creating the file check its size and block usage with
  ls -ls file_with_hole
  You will see that it is 131082 bytes but does not use more than a few blocks.


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


******************************************************************************/
#define _GNU_SOURCE
#include "../include/common_hdrs.h"
#include "../include/common_defs.h"

#define FILENAME       "file_with_hole"
#define MESSAGE_SIZE   512
#define BUFFER_SIZE     10

int main(int argc, char *argv[])
{
    int     fd;
    char    buffer[BUFFER_SIZE];
    char    message[MESSAGE_SIZE];

    /* Create a new file named file_with_hole in the pwd. */
    if ((fd = open(FILENAME, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0) {
        sprintf(message, "unable to open file %s for writing", FILENAME);
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
        sprintf(message, "error closing file %s\n", FILENAME);
        fatal_error(errno, message);
      }

    /* We now have a large file with a big hole. */
    exit(EXIT_SUCCESS);
}
