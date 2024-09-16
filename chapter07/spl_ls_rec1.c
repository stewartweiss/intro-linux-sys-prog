/****************************************************************************
  Title          : spl_ls_rec.c
  Author         : Stewart Weiss
  Created on     : September 29, 2023
  Description    : Lists directory contents, recursively
  Purpose        : To do a tree walk by recursion on opendir()
  Usage          : spl_ls_rec [file file ...]
                   where files may be any file type including directories
  Build with     : gcc -Wall -g -I ../include spl_ls_rec.c -o spl_ls_rec \
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
#include "dir_utils.h"

void listdir( DIR *dirp, char *dirname,  int flags )
{
    struct dirent *direntp;       /* Pointer to directory entry structure  */
    BOOL          done = FALSE;   /* Flag to control loop execution        */
    char          pathname[PATH_MAX];  /* Pathname of file to open         */
    DIR           *subdirp;       /* Dir stream for subdirectory           */
    char*         name;

    while ( !done ) {
        errno = 0;
        direntp = readdir( dirp );                       /* Get next entry. */
        if ( direntp == NULL && errno != 0 )
            /* Not the end of the stream, but an error from readdir()       */
            perror("readdir");
        else if ( direntp == NULL )
            /* Implies errno == 0, so it's the end of the stream.           */
           done = TRUE;
        else {
            name = direntp->d_name;
            if ((strcmp(name, ".") != 0) && (strcmp(name, "..") != 0)) {
                sprintf(pathname, "%s/%s", dirname, name);
                printf("%s\n",pathname);
                if ( isdir(direntp) ) {
                    errno = 0;
                    if ( ( subdirp = opendir(pathname) ) == NULL ) {
                        error_mssge(errno, name);
                    }
                    else {
                        listdir(subdirp, pathname, flags);
                        closedir(subdirp);
                    }
                }
            }
        }
    }
    printf("\n");
}

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
           listdir( dirp, ".",  ls_flags );
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
                listdir( dirp, argv[i], ls_flags);
                closedir(dirp);
            }
        }
    }
    return 0;
}

