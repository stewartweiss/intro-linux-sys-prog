/*****************************************************************************
  Title          : spl_ps.c
  Author         : Stewart Weiss
  Created on     : March 24, 2024
  Description    : A simplified ps command
  Purpose        : To show how to use /procfs for accessing process stats
  Usage          : spl_ps
  Build with     : gcc -Wall -o spl_ps -I../include -L../lib spl_ps.c \
                      -lspl -lm -lrt

******************************************************************************
* Copyright (C) 2024 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/
#include "common_hdrs.h"
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include "ps_utils.h"


void printallprocs( DIR *dirp )
{
    struct dirent   *direntp;     /* Pointer to directory entry structure   */
    char*   accepts="0123456789"; /* For matching directory names           */
    char    pathname[PATH_MAX];   /* Pathname to file to open               */
    char    heading[MAX_LINE];    /* String containing heading              */
    char    psline[MAX_LINE];     /* String containing one proc's data      */
    size_t  len = MAX_LINE;       /* Length of line getline() returned      */
    FILE*   fp;                   /* File stream to read                    */
    char*   buf;
    procstat  ps_fields;
    struct  stat  statbuffer;

    memset(heading,0, MAX_LINE);
    printheadings(heading);
    printf("%s", heading);
    if ( NULL == (buf = malloc(MAX_LINE))) /* Allocate buffer for getline() */
        fatal_error(errno, "malloc");

    while ( TRUE ) {
        errno = 0;
        direntp = readdir( dirp );                       /* Get next entry. */
        if ( direntp == NULL && errno != 0 )
            perror("readdir"); /* Not end of stream, but a readdir() error  */
        else if ( direntp == NULL )   /* The end of the stream was reached  */
            break;
        else if (strspn(direntp->d_name, accepts) == strlen(direntp->d_name))
        {   /* Directory name is a number. */
            memset(pathname, '\0', PATH_MAX);
            sprintf(pathname, "/proc/%s/stat",direntp->d_name);
            if ( NULL == (fp = fopen(pathname, "r") ))
                fatal_error(errno, "fopen");     /* Error opening stat file */
            if ( -1 == fstat(fileno(fp), &statbuffer))
                fatal_error(errno, "fstat");    /* Error stat-ing stat file */
            ps_fields.uid = statbuffer.st_uid;
            if ( -1 == getline(&buf,&len, fp ) )
                fatal_error(errno, "getline()");
            parse_buf(buf, &ps_fields);
            print_one_ps(ps_fields, psline);
            printf("%s", psline);
        }
    }
    free(buf);
    printf("\n");
}


int main(int argc, char* argv[])
{
    DIR   *dirp;

    get_hertz();
    errno = 0;
    if ( ( dirp = opendir("/proc") ) == NULL )
        fatal_error(errno, "opendir");           /* Could not open cwd. */
    else
        printallprocs(dirp);
    exit(EXIT_SUCCESS);
}
