/*****************************************************************************
  Title          : spl_pwd.c
  Author         : Stewart Weiss
  Created on     : October 28, 2023
  Description    : Implements pwd across mount points
  Purpose        :
  Usage          : spl_pwd [-v]
                   -v turns on verbose output
  Build with     : gcc -g -Wall -o spl_pwd spl_pwd.c -L../lib  -lutils  \
                   -I../include -lspl


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

#include  "common_hdrs.h"
#include  <sys/stat.h>
#include   <dirent.h>

/* A structure to store an (inode number, device ID) pair */
typedef struct device_inode_pair
{
    dev_t  dev;
    ino_t  ino;
} dev_ino;


/*****************************************************************************
                          Utility Function Prototypes
*****************************************************************************/

/* are_samefile(di1, di2) returns TRUE iff the device-inode pair di1 is equal
   to device inode pair di2. */
BOOL  are_samefile(dev_ino f1, dev_ino f2)
{
   return ( f1.ino == f2.ino && f1.dev == f2.dev );
}


/*  get_dev_ino() gets the device id and the inode number of the file whose
    pathname is given in its first argument.
    Returns 0 if successful, or an err value if it fails.
*/
void  get_dev_ino( const char *fname, dev_ino *dev_inode)
{
    struct stat sb;
    errno = 0;
    if ( -1 == lstat( fname , &sb )  )
        fatal_error(errno, "Cannot stat ");
    dev_inode->dev  = sb.st_dev;
    dev_inode->ino  = sb.st_ino;
}


/* get_filename(e) returns the filename of the directory entry e
  in the current working directory. */
char* get_filename( dev_ino child_entry)
{
    DIR           *dir_ptr;     /* The directory to be opened         */
    struct dirent *direntp;     /* The dirent for each entry          */
    dev_ino       this_entry;   /* The dev_ino pair for the entry     */
    char          errmssge[256];/* To store error messages            */
    int           len;          /* Length of a string                 */
    char*         fname;        /* malloc-ed name to return to caller */

    errno = 0;
    dir_ptr = opendir( "." );
    if ( dir_ptr == NULL )
        fatal_error(errno, "opendir" );

    /* Search through the current working directory for a file whose
       inode number and device ID are that of entry.*/
    while ( ( direntp = readdir( dir_ptr ) ) != NULL ) {
        errno = 0;
        get_dev_ino(direntp->d_name, &this_entry);

        /* If this entry matches, we found the file. */
        if ( are_samefile( this_entry, child_entry) ) {
            /* Copy the entry's d_name into a malloc-ed fname. */
            len = strlen(direntp->d_name);
            errno = 0;
            if ( NULL == (fname = calloc(len+1,1)) )
                fatal_error(errno, "malloc");
            strncpy(fname, direntp->d_name, len);

            closedir( dir_ptr );
            return fname;
        }
    }
    /* If we reach here, there is no matching entry in this directory */
    sprintf(errmssge, "i-number %lu not found.\n", child_entry.ino);
    error_mssge(-1, errmssge);
    return NULL;
}


int main(int argc, char* argv[])
{
    dev_ino     current;            /* dev-ino pair for node to find        */
    dev_ino     root;               /* dev-ino pair for root directory      */
    char        pathname[PATH_MAX]; /* Buffer to store pathname             */
    char        *dirname;           /* Next component of pathname to find   */
    ssize_t     front;              /* Leftmost position of current pathname*/
    ssize_t     namelength;         /* Length of next component in pathname */

    get_dev_ino("/", &root );
    get_dev_ino( ".", &current);
    front = PATH_MAX-1;
    pathname[PATH_MAX-1] = '\0';

    while ( !are_samefile(current, root)) {
        /* go up to parent directory */
        chdir( ".." );
        /* Search in the parent directory for the fileame of this_inode. */
        if ( NULL == (dirname = get_filename(current)) )
            fatal_error(-1,
                   "Could not find entry in .. for current directory.");
        /* If successful, write this name to the left of the current path. */
        namelength = strlen(dirname);

        /* Check if the new path is too long. If so, fill with dots instead
           and report the error. */
        if ( front - namelength <= 0 ) {
            memset(&(pathname[0]), '.', front);
            front = 0;
            error_mssge(ENAMETOOLONG, "Error");
            break;
        }
        else {
            front = front - namelength;
            memcpy(pathname+front, dirname, namelength);
        }
        /* Free the memory allocated by get_filename for this string. */
        free(dirname);
        pathname[front-1] = '/';
        front--;
        get_dev_ino(".", &current);
    }
    printf("%s\n", &(pathname[front]));
    return 0;
}
