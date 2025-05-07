/*****************************************************************************
  Title          : lsdir.c
  Author         : Stewart Weiss
  Created on     : September 25, 2023
  Description    : Simple ls command.
  Purpose        : To show how to read the contents of a directory
  Usage          : lsdir [file ...]
  Build with     : gcc -Wall -g -L"../lib" -o  lsdir lsdir.c -lspl
  Modifications  :
  Notes          : For directories, displays their contents, including
                   . and ..; for files, just prints their names.

******************************************************************************
 * Copyright (C) 2025 - Stewart Weiss
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

*****************************************************************************/

#include <sys/types.h>
#include <dirent.h>
#include "../include/common_hdrs.h"

/** listdir(dirstream) prints the filenames contained in the directory
*   pointed to by dirstream, one per line, including . and .., in
*   the order determined by the stream itself.
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
                closedir(dirp);
                closedir(dirp);
    return 0;
}
