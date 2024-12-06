/*****************************************************************************
  Title          : shm_user_demo1.c
  Author         : Stewart Weiss
  Created on     : June 7, 2024
  Description    : A reader program that reads shared memory
  Purpose        : To introduce basics of shared memory
  Usage          : shm_user_demo1  /<shared-memory-name>
  Build with     : gcc -Wall -g -I../include -L../lib  -o shm_user_demo1 \
                    shm_user_demo1.c -lrt

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

#include "shm_demo1.h"  /* For definitions of constants */


int main(int argc, char *argv[])
{
    int fd;            /* File descriptor referring to shared memory object */
    char *shmp;        /* Pointer to start of shared memory region          */
    char  usage[256];  /* For error message                                 */

    if ( (argc != 2) || (argv[1][0] != '/') ){
        sprintf(usage, "Usage: %s /shm-name\n", argv[0]);
        usage_error(usage);
    }
    /* Open the named shared memory object for reading and writing.    */
    if ( (fd = shm_open(argv[1],  O_RDWR,0 ))  == -1 )
        fatal_error(errno, "shm_open");

     /* Map the object into the process's address space.               */
    if ( ( shmp = mmap(NULL, BUF_SIZE, PROT_READ|PROT_WRITE,
                      MAP_SHARED, fd, 0) ) == MAP_FAILED )
        fatal_error(errno,"mmap");

    if ( -1 == close(fd))  /* Close unneeded file descriptor.          */
        fatal_error(errno, "close");

    printf("Memory is mapped in process starting at address %p\n", shmp);

    /* Print the two strings that are supposed to be in the locations
       offset1 and offset2 away from the region start. */
    printf("%s\n", shmp+offset1);
    printf("%s\n", shmp+offset2 );

    shm_unlink(argv[1]); /* Remove this reference to the memory object. */
    exit(EXIT_SUCCESS);
 }


