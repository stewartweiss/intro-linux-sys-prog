/******************************************************************************
  Title          : spl_du1.c
  Author         : Stewart Weiss
  Created on     : November 1, 2023
  Description    : Directory hierarchy traversal
  Purpose        : To show a simple application of the nftw function
  Usage          : spl_du1  file file ...
  Build with     : gcc -Wall -g -I ../include spl_du1.c -o spl_du1 \
                   -L../lib -lspl
  NOTES:
  This walks the directory tree for each file argument, displaying file name
  and type and accumulating total bytes in the tree.
  THIS COUNTS NAMES OF THE SAME FILE MULTIPLE TIMES. IT NEEDS TO DETECT
  WHEN A FILE HAS BEEN COUNTED ALREADY.

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
#include <stdint.h>
#include <limits.h>

#define  MAXDEPTH  100

/*
  totalsize is an array with an entry for each level in the tree.
  It stores the total number of bytes for that level within the currently
  processed tree. To be precise, if the level being processed is k,
  then totalsize[k] contains the sum of the sizes of all files visited so
  far in the tree rooted at level k-1 containing the files currently
  visited, including those in all subtrees of those trees already visited.
*/
static  uintmax_t totalsize[MAXDEPTH];



int file_usage(const char *fpath, const struct stat *sb,
                 int tflag, struct FTW *ftwbuf)
{
    /*
       We maintain two indices, prev_level and cur_level. At any time
       cur_level is the level of the file currently being processed,
       and if prev_level != -1, then it is the level of the file
       processed immediately before the current file.
    */
    static int prev_level = -1;
    int        cur_level;

    /* the size of the current object. If it is a direcory, it includes the
       sum of all descendants. This is what we print next to the name */
    uintmax_t  cur_usage;

    /* Store the level of the current file into cur_level */
    cur_level = ftwbuf->level;
    if ( cur_level >= MAXDEPTH ) {
        fprintf(stderr, "Exceeded maximum depth.\n");
        return -1;
    }
    /*
       If prev_level == cur_level then we are visiting a file that is a
       sibling of the one previously visited. In this case, we add its size
       to the current total size for this level and we set cur_size to its
       size.
    */
    if ( prev_level == cur_level ) {
        cur_usage = sb->st_blocks/2;
        totalsize[cur_level] += cur_usage;
    }

    /*
       If prev_level > cur_level then we have just returned in the post-
       order traversal to a directory all of whose children have been
       visited. It can only be the case that prev_level == cur_level+1
       but we do not need this fact.

       The totalsize for the current level is now the sum of the previous
       value of totalsize plus the size of the directory object itself
       plus the sum of the sizes of all children just visited. We can
       prove by induction that totalsize[prev_level] is exactly this sum,
       since after each child of the current node was visited, it was
       equal to the sum of all nodes in its tree plus those of its siblings
       that had already been visited.

       We set cur_size to totalsize[prev_level] + sb->st_blocks/2, which
       is the size of this directory, and we add this to totalsize[cur_level].
       We then zero out totalsize[prev_level]. This is explained in the notes.
    */
    else if ( prev_level > cur_level ) {
        cur_usage    = totalsize[prev_level] + sb->st_blocks/2;
        totalsize[cur_level] += cur_usage;
        totalsize[prev_level] = 0;
    }

    /*
       The last case is that prev_level < cur_level. This implies that we
       descended from the previously visited node, which can only happen
       during a post-order traversal when we reach a leaf node that is
       leftmost in its tree (or the first node to be visited in that tree).
       In this case we reset the totalsize for that level to 0 and add
       that file's size to it. Equivalently, we set totalsize[cur_level]
       to the size of that object.
    */
    else {
        cur_usage = sb->st_blocks/2;
        totalsize[cur_level] = cur_usage;
    }

    /* Print out the size and then the name of the file */
    printf("%ju\t%s", cur_usage, fpath);

    /* now make prev_level the current level */
    prev_level = cur_level;

    /* Check flags and print a message if need be */
    if ( tflag == FTW_DNR )
        printf(" (unreadable directory)");
    else if ( tflag == FTW_SL )
        printf(" (symbolic link)" );
    else if ( tflag == FTW_SLN )
        printf(" (broken symbolic link)" );
    else if ( tflag == FTW_NS )
        printf("stat failed " );
    printf("\n");

   return 0;           /* To tell nftw() to continue */
}


int main(int argc, char *argv[])
{
    int flags = FTW_DEPTH | FTW_PHYS | FTW_MOUNT;
    int status;
    int i = 1;

    if ( argc < 2 )  {
        memset( totalsize, 0, MAXDEPTH*sizeof(uintmax_t));
        if ( 0 != (status = nftw(".", file_usage, 20, flags) ) )
           fatal_error(status, "nftw");
    }
    else
        while (i < argc) {
            memset( totalsize, 0, MAXDEPTH*sizeof(uintmax_t));
            if ( 0 != ( status = nftw(argv[i], file_usage, MAXDEPTH, flags)))
                fatal_error(status, "nftw");
            else
                i++;
        }
    exit(EXIT_SUCCESS);
}
