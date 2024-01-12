/****************************************************************************
  Title          : lsdirsfirst.c
  Author         : Stewart Weiss
  Created on     :
  Description    :
  Purpose        :
  Usage          :
  Build with     :
  Modifications  :

*****************************************************************************

Copyright (C) 2023 - Stewart Weiss

This code is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

****************************************************************************/

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#include "common_hdrs.h"
#include "dir_utils.h"

#define  LIST_DIRS_FIRST  1


/** listdir_dirsfirst(dirstream) prints the filenames contained in the
*   directory pointed to by dirstream, one per line, including . and ..,
*   such that all entries that are directories are printed before any
*   non-directory entries.
*/

void listdir(DIR *dirp, int flags)
{
    struct dirent *entry;     /* Pointer to directory entry structure      */
    long int      pos;        /* Position of entry, returned by telldir()  */
    long          *saved_positions; /* Array of saved positions.           */
    long          *temp;
    int           count = 0; /* To keep track of array size               */
    int           i = 0;
    int           MAX_ENTRIES = 512; /* Initial array size.               */

    /* Allocate array in heap. */
    saved_positions = (long*) malloc(sizeof(long)*MAX_ENTRIES);
    while ( TRUE ) {
        pos = telldir(dirp);       /* Save current position.     */
        errno = 0;                 /* Try to read entry.         */
        if ( NULL == (entry = readdir(dirp) ) && errno != 0 )
            perror("readdir"); /* Error reading entry.       */
        else if ( entry == NULL )
            break;
        else if ( flags & LIST_DIRS_FIRST ) {
                if ( ! isdir(entry) ) {
                    /* It's not a directory, so save it for printing later. */
                    if ( count < MAX_ENTRIES ) {
                        saved_positions[count++] = pos;
                    }
                    else {
                        /* Out of memory. Allocate array 4 times larger. */
                        MAX_ENTRIES= 4*MAX_ENTRIES;
                        errno = 0;
                        if ( NULL == (temp = (long*) realloc(saved_positions,
                                    sizeof(long)*MAX_ENTRIES) ))
                            fatal_error(errno, "Over limit on subdirectories");
                        saved_positions = temp;
                        saved_positions[count++] = pos;
                    }
                }
                else
                    printf("%s/\n", entry->d_name);
            }
        else
            printf("%s/\n", entry->d_name);
    }
    /* Post-process list of saved filenames */
    if ( count > 0 ) {
        while( i < count ) {
            seekdir(dirp, saved_positions[i]); /* Restore position in dirp. */
            entry = readdir(dirp);             /* Read entry.               */
            printf("%s\n", entry->d_name);     /* Print filename.           */
            i++;
        }
    }
    free(saved_positions);
}

int main(int argc, char *argv[])
{
    DIR   *dirp;
    int    i;

    if ( 1 == argc ) {   /* No arguments; use current working directory. */
        errno = 0;
        if ( ( dirp = opendir(".") ) == NULL )
            fatal_error(errno, "opendir"); /* Could not open cwd. */
        else
           listdir( dirp ,1);
    }
    else {
        i = 1;
        /* For each command-line argument, call opendir() on it. */
        while ( i < argc ) {
            errno = 0;
            if ( ( dirp = opendir(argv[i]) ) == NULL ) {
                if ( errno == ENOTDIR )  /* It's not a directory. */
                    printf("%s\n", argv[i] );
                else  /* It's an error. */
                    error_mssge(errno, argv[i]);
            }
            else {  /* A successful open of a directory */
                printf("%s:\n", argv[i] );
                listdir( dirp,1 );
                closedir(dirp);
            }
            i++; /* Advance to next argument. */
        }
    }
    return 0;
}

