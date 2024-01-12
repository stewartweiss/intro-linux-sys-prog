/*****************************************************************************
  Title          : spl_ls1.c
  Author         : Stewart Weiss
  Created on     : September 25, 2023
  Description    : Simple ls command.
  Purpose        : To show how to read the contents of a directory
  Usage          : spl_ls1 [file ...]
  Build with     : gcc -Wall -g -I../include -L../lib -o spl_ls1 spl_ls1.c  \
                     -lspl
  Modifications  :
  Notes          : For directories, displays their contents, including
                   . and ..; for files, just prints their names.
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

#include <sys/types.h>
#include <dirent.h>
#include "common_hdrs.h"

/* listdir(dirp,flag) prints the filenames contained in the directory stream
   dirp, one per line, including . and .., in  the order the stream delivers
   them.
*/
void listdir( DIR *dirp, int flags )
{
    struct dirent   *direntp;      /* Pointer to directory entry structure  */
    BOOL            done = FALSE;  /* Flag to control loop execution        */

    while ( !done ) {
        errno = 0;
        direntp = readdir( dirp );                       /* Get next entry. */
        if ( direntp == NULL && errno != 0 )
            /* Not the end of the stream, but an error from readdir()       */
            perror("readdir");
        else if ( direntp == NULL )
            /* Implies errno == 0, so it's the end of the stream.           */
           done = TRUE;
        else
           printf("    %s\n", direntp->d_name );          /* Print it.      */
    }
    printf("\n");
}

/*****************************************************************************
                               Main Program
*****************************************************************************/
int main(int argc, char *argv[])
{
    DIR   *dirp;
    int    i;
    int    ls_flags = 0;

    if ( 1 == argc ) {      /* No arguments; use current working directory. */
        errno = 0;
        if ( ( dirp = opendir(".") ) == NULL )
            fatal_error(errno, "opendir");           /* Could not open cwd. */
        else
           listdir( dirp, ls_flags );
    }
    else {         /* For each command-line argument, call opendir() on it. */
        for ( i = 1; i < argc; i++) {
            errno = 0;
            if ( ( dirp = opendir(argv[i]) ) == NULL ) {
                if ( errno == ENOTDIR )  /* It's not a directory. */
                    printf("%s\n", argv[i] );
                else  /* It's an error. */
                    error_mssge(errno, argv[i]);
            }
            else {  /* A successful open of a directory */
                printf("\n%s:\n", argv[i] );
                listdir( dirp, ls_flags);
                closedir(dirp);
            }
        }
    }
    return 0;
}
