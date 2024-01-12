/****************************************************************************
  Title          : spl_ls_rec2.c
  Author         : Stewart Weiss
  Created on     : September 29, 2023
  Description    : Lists directory contents, recursively
  Purpose        : To do a tree walk by recursion on opendir()
  Usage          : spl_ls_rec2 [file file ...]
                   where files may be any file type including directories
  Build with     : gcc -Wall -g -I ../include spl_ls_rec2.c -o spl_ls_rec2 \
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
#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#include "common_hdrs.h"
#include "dir_utils.h"


int scan_one_dir(const char* dirname )
{
    struct dirent **namelist;
    int i, n;
    char   pathname[PATH_MAX];

    errno = 0;
    if ( (n = scandir(dirname, &namelist, NULL, alphasort) ) < 0){
        fatal_error(errno, "scandir");
    }
    for (i = 0; i < n; i++) {
        if (strcmp(namelist[i]->d_name, ".") != 0
            && strcmp(namelist[i]->d_name, "..") != 0) {
            printf("%s/%s\n",dirname,namelist[i]->d_name);
            if ( isdir(namelist[i]) ) {
                sprintf(pathname, "%s/%s", dirname, namelist[i]->d_name);
                scan_one_dir(pathname);
            }
        }
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

    if ( 1 == argc )
        scan_one_dir(".");
    else {
        int i;
        for (i = 1; i < argc; i++ ) {
            printf("%s:\n", argv[i]);
            scan_one_dir(argv[i]);
            if ( i < argc-1 ) printf("\n");
        }
    }
    exit(EXIT_SUCCESS);
}

