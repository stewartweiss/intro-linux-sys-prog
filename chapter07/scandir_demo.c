/****************************************************************************
  Title          : scandir_demo.c
  Author         : Stewart Weiss
  Created on     : September 29, 2023
  Description    : Lists all directories in a given directory
  Purpose        : To work with various functions in directory API
  Usage          : scandir_demo [dir dir ...]
  Build with     : gcc -Wall -g -I ../include scandir_demo.c -o scandir_demo \
                   -L../lib -lspl
  Modifications  : 10/31/25 by SNW
                   Removed the isdot() function, which was unused here.

                   Replaced the while-loop in scan_one_dir(), which had an
                   Off-by-one error in its entry condition, by a for-loop.

                   Changed the description in this preamble to match what this
                   program does, which is to display only those entries in a
                   given directory that are directories.

                   Rewrote the pickdir() function to make it a wrapper of
                   isdir(), since before they were redundant.

******************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#define _BSD_SOURCE
#include "common_hdrs.h"
#include <dirent.h>


/* A function to determine if a directory entry is a directory */
/* Check whether we have d_type in struct. If so use it.
   If not, call stat() and check st_mode for file type. */
BOOL isdir(  const struct dirent *direntp)
{
#ifdef _DIRENT_HAVE_D_TYPE
    if( direntp->d_type  == DT_DIR)
#else
    struct stat    statbuf;
    stat(direntp->d_name, &statbuf);
    if ( S_ISDIR(statbuf.st_mode) )
#endif
        return TRUE;
    else
        return FALSE;
}

/* A very thin wrapper for isdir() that can be passed as the third
   argument to scandir() */
int pickdir(  const struct dirent *direntp)
{
   return (int) (isdir(direntp));
}

/** dirsfirstsort(&d1, &d2) compares the direct structures pointed to by
*   d1 and d2 and returns -1, 0, or 1 according to the sort order:
*   if both are directories, return
*       -1 if d1 <= d2 by the string collating order
*        1 if d2 > d1 by the string collating order
*   else
*       -1  if d1 is a directory and d2 is not
*        1  if d1 is not a directory and d2 is
*   else if neither is a directory, return -1, 0, or 1 if d1 < d2,
*         d1 == d2, or d1 > d2 respectively.
*/
int dirsfirstsort(const struct dirent **a, const struct dirent **b)
{
    if ( isdir(*a) )
        if (! isdir(*b) )
            return -1;
        else
            return (alphasort(a,b));
    else
        if ( isdir(*b) )
            return 1;
        else
            return (alphasort(a,b));
}

void print( const struct dirent* direntp )
{
        printf("%s", direntp->d_name);
        if ( isdir(direntp) )
            printf("/");
        printf("\n");
}


int scan_one_dir(const char* dirname, void (*process )(const struct dirent* ))
{
    struct dirent **namelist;
    int n;

    errno = 0;
    if ( (n = scandir(dirname, &namelist, pickdir, dirsfirstsort) ) < 0){
        fatal_error(errno, "scandir");
    }

    for (int i = 0; i < n; i++) {
        process(namelist[i]);
        free(namelist[i]);
    }
    free(namelist);
    return(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{
    if ( 1 == argc )
        scan_one_dir(".", print);
    else {
        int i = 1;
        while ( i < argc ) {
            printf("\n%s:\n", argv[i]);
            scan_one_dir(argv[i++], print);
        }
    }
    exit(EXIT_SUCCESS);
}

