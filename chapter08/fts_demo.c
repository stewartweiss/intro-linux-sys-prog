/******************************************************************************
  Title          : fts_demo.c
  Author         : Stewart Weiss
  Created on     : December 28, 2023
  Description    : Displays files with sizes and finds largest file
  Purpose        : To demonstrate how to use the fts set of functions
  Usage          : fts_demo dirname
                   where dirname is any directory
  Build with     : gcc -Wall -g -I../include fts_demo.c -L../lib -lutils -o \
                        fts_demo

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

#include "common_hdrs.h"
#include <fts.h>

int namecmp(const FTSENT **s1, const FTSENT **s2)
{
    return (strcoll((*s1)->fts_name, (*s2)->fts_name));
}

int main(int argc, char* argv[])
{
    FTS *tree;
    FTSENT *file;
    char errmssge[128];
    char largest_file[PATH_MAX];
    size_t  max = 0, size;

    if ( argc < 2 ) {
        sprintf(errmssge,"%s directory\n", argv[0]);
        usage_error(errmssge);
    }

    char *dir[] = { argv[1],  NULL };

    if ( NULL == (tree = fts_open(dir, FTS_PHYSICAL , namecmp)))
        fatal_error(errno, "fts_open");

    errno = 0;
    while ((file = fts_read(tree))) {
        switch (file->fts_info) {
        case FTS_DNR:  /* Cannot read directory */
            fprintf(stderr, "Could not read %s\n", file->fts_path);
            continue;
        case FTS_ERR:  /* Miscellaneous error */
            fprintf(stderr, "Error on %s\n", file->fts_path);
            continue;
        case FTS_NS:  /* stat() error - display message and continue . */
            fprintf(stderr, "Could not stat %s\n", file->fts_path);
            continue;
        case FTS_DP:  /* Ignore post-order visit to directory. */
            continue;
        }
        size = file->fts_statp->st_size;
        if ( max < size ) {
            max = size;
            strncpy(largest_file, file->fts_path, 1+file->fts_pathlen);
        }
        printf("%12ld\t%*s%s\n", size,
                4*(file->fts_level), " ", file->fts_path);
        errno = 0;
    }
    if (errno != 0)
        fatal_error(errno, "fts_read");

    printf("Largest file: %s; Size=%lu\n", largest_file, max);
    if (fts_close(tree) < 0)
      fatal_error(errno, "fts_close");
    return(EXIT_SUCCESS);
}

