/****************************************************************************
  Title          : spl_ls3.c
  Author         : Stewart Weiss
  Created on     : September 30, 2023
  Description    : Lists directory contents, sorting directories before files
  Purpose        : To show how to use scandir()
  Usage          : spl_ls3 [file file ...]
                   where files may be any file type including directories
  Build with     : gcc -Wall -g -I ../include spl_ls3.c -o spl_ls3 \
                   -L../lib -lspl

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

#define _BSD_SOURCE
#include "common_hdrs.h"
#include "dir_utils.h"   /* Needed for isdir() and dirsfirstsort() */


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

/** print(dp) prints the filename of entry *dp. If it's a directory, it
    appends a trailing '/' to its name. */
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
    if ( (n = scandir(dirname, &namelist, NULL, dirsfirstsort) ) < 0){
        fatal_error(errno, "scandir");
    }

    int i;
    for (i = 0; i < n; i++) {
        process(namelist[i]);
        free(namelist[i]);
    }
    free(namelist);
    return(EXIT_SUCCESS);
}


int main(int argc, char* argv[])
{

    if ( setlocale(LC_TIME, "")  == NULL )
        fatal_error( LOCALE_ERROR,
                 "setlocale() could not set the given locale");

    if ( 1 == argc ) /* If no arguments, list the CWD.                    */
        scan_one_dir(".", print);
    else {           /* Otherwise, for each argument, scan it.            */
        int i;
        for (i = 1; i < argc; i++ ) {
            printf("\n%s:\n", argv[i]);     /* Print the argument.        */
            scan_one_dir(argv[i], print);   /* pass the print() function. */
            if ( i < argc-1 ) printf("\n"); /* Put a newline before next. */
        }
    }
    exit(EXIT_SUCCESS);
}



