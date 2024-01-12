/*****************************************************************************
  Title          : nftw_demo.c
  Author         : Stewart Weiss
  Created on     : December 18, 2023
  Description    : Directory hierarchy traversal
  Purpose        : To show a simple application of the nftw function
  Usage          : nftw_demo [options] file file ...
                   where options may be -c -p -d -m
                   -c : change directory as it visits directories
                   -p : do not follow symbolic links
                   -d : do a post-order instead of pre-order (default)
                   -m : do not cross mount points

  Build with     : gcc -Wall -g -I../include nftw_demo.c -L../lib -lutils -o \
                        nftw_demo
  Notes:
  This program traverses the file hierarchy rooted at the filename given as
  its command line argument, or if it is absent, rooted in the current working
  directory. It accepts several different options, for the sole purpose of
  showing how these options affect the behavior of nftw() and the values that
  it passes to the function that it calls at each tree node.

  To illustrate one simple application of nftw(), this program lists the
  files that it finds, one per line, indented in proportion to its depth in
  the tree, so that it implicitly shows which files are contained in which
  directories.

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
#include <ftw.h>
#define  MAXOPENFD   20       /* Maximum number of file descriptors to open */

int display_info(const char *fpath, const struct stat *sb,
             int tflag, struct FTW *ftwbuf)
{
    char  indent[PATH_MAX];                      /* A blank string */
    const char *basename = fpath + ftwbuf->base; /* Filename of entry */
    int   width          = 4*ftwbuf->level;      /* Length of leading path */

    /* Fill indent with a string of 4*level blanks.   */
    memset(indent, ' ', width);
    indent[width] = '\0';

    /* Print out blanks then filename (not full path).*/
    printf("%s%-30s", indent, basename );

    /* Check flags and print a message if need be.    */
    if ( tflag == FTW_DNR )
        printf(" (unreadable directory)");
    else if ( tflag == FTW_SL )
        printf(" (symbolic link)" );
    else if ( tflag == FTW_SLN )
        printf(" (broken symbolic link)" );
    else if ( tflag == FTW_NS )
        printf(" (stat failed) " );

    printf("\n");
    return 0;               /* Tell nftw() to continue. */
}
int main(int argc, char *argv[])
{
    int flags = 0;
    int ch;
    char options[] = ":dpm"; /* Three possible options */
    opterr = 0;

    while  (TRUE) {
        if ( -1 == (ch = getopt(argc, argv, options)) )
            break;
        switch ( ch ) {
        case 'd':  flags |= FTW_DEPTH;    break;
        case 'p':  flags |= FTW_PHYS;     break;
        case 'm':  flags |= FTW_MOUNT;    break;
        default:   fprintf (stderr, "Bad option found.\n");
            return 1;
        }
    }
    errno = 0;
    if (optind < argc)
        while (optind < argc) {
            if ( -1 == nftw(argv[optind], display_info, MAXOPENFD, flags))
                fatal_error(errno, "nftw");
            optind++;
        }
    else if ( -1 == nftw(".", display_info, MAXOPENFD, flags) )
            fatal_error(errno, "nftw");
    else
        exit(EXIT_SUCCESS);
}
