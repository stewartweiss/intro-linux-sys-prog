/*****************************************************************************
  Title          : spl_cp2.c
  Author         : Stewart Weiss
  Created on     : August 4, 2023
  Description    : Modified version of copy.c with buffersize argument
  Purpose        : To test the effect of buffer size on performance
  Usage          : spl_cp2 sourcefile targetfile buffersize
  Build with     : gcc -Wall -g -I../include -L ../lib -lspl \
                   -o spl_cp2  spl_cp2.c

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
#define PERMISSIONS    S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH  /*rw-rw-r-- */

int main(int argc, char *argv[])
{
    int     source_fd;
    int     target_fd;
    int     num_bytes_read;
    int     num_bytes_written;
    mode_t  permissions = PERMISSIONS;
    char    message[MESSAGE_SIZE];
    int     retval;
    long    bufsize;
    char    *buffer;

    /* Check for correct usage.                                             */
    if ( argc != 4 ){
        sprintf(message,"%s source destination buffer-size", basename(argv[0]));
        usage_error(message);
    }

    /* Open source file for reading.                                        */
    errno = 0;
    if ( (source_fd = open(argv[1], O_RDONLY)) == -1 ) {
        sprintf(message, "unable to open %s for reading", argv[1]);
        fatal_error(errno, message);
    }

    /* Open target file for writing.                                        */
    errno = 0;
    if ( (target_fd = open( argv[2], O_WRONLY|O_CREAT|O_TRUNC,
                            permissions) ) == -1 ) {
        sprintf(message, "unable to open %s for writing", argv[2]);
        fatal_error(errno, message);
    }

    /* Get the buffersize.                                                  */
    if ( (retval = get_long(argv[3], NON_NEG_ONLY, &bufsize, message )) < 0)
        fatal_error(retval, message);

    buffer = malloc((size_t) bufsize);
    if ( buffer == NULL )
        fatal_error(errno, "malloc");

    /* Start copying:
       Transfer BUFFER_SIZE bytes at a time from source_fd to target_fd.    */
      errno = 0;
      while ( (num_bytes_read = read(source_fd , buffer, bufsize)) > 0 ){
          errno = 0;
          num_bytes_written = write( target_fd, buffer, num_bytes_read ) ;
          if ( errno != 0 )
              fatal_error(errno, "copy");
          else
              if ( num_bytes_written  != num_bytes_read) {
                  sprintf(message,"write error to %s\n", argv[2]);
                  fatal_error(-1, message);
              }
          errno = 0;
      }
      if (num_bytes_read == -1)
          fatal_error(errno, "error reading");

      /* Close files.                                                       */
      errno = 0;
      if ( close(source_fd) == -1 ) {
          sprintf(message, "error closing source file %s", argv[1]);
          fatal_error(errno, message);
      }
      if ( close(target_fd) == -1 ) {
          sprintf(message, "error closing target file %s", argv[2]);
          fatal_error(errno, "error closing target file");
      }
      return 0;
  }



