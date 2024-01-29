/****************************************************************************
  Title          : spl_ls2.c
  Author         : Stewart Weiss
  Created on     : September 29, 2023
  Description    : Lists directory contents, sorting directories before files
  Purpose        : To work with various functions in directory API
  Usage          : spl_ls2 [file file ...]
                   where files may be any file type including directories
  Build with     : gcc -Wall -g -I ../include spl_ls2.c -o spl_ls2 \
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
#include <dirent.h>

#define  LIST_DIRS_FIRST  1

/* A function to determine if a directory entry is a directory */
/* Check whether we have d_type in struct. If so use it.
   If not, call stat() and check st_mode for file type. */
BOOL isdir( const struct dirent *direntp)
{
#ifdef _DIRENT_HAVE_D_TYPE
    if( direntp->d_type  == DT_DIR)
#else
    struct stat    statbuf;
    stat(direntp->d_name, &statbuf);
    if ( S_ISDIR(statbuf.st_mode)  )
#endif
        return TRUE;
    else
        return FALSE;
}

/* List Functions */
struct listnode{
    long pos;
    struct listnode *next;
};
typedef struct listnode poslist;

void save(long pos,  poslist **list)
{
    poslist* pos_node_ptr;
    static   poslist* last = NULL;

    /* Allocate a new list node. */
    errno = 0;
    if ( NULL == (pos_node_ptr = (poslist*) malloc(sizeof(poslist)) ) )
        fatal_error(errno, "malloc");

    /* Copy the position into the new node. */
    pos_node_ptr->pos = pos;

    /* Attached the node to the end of the list. */
    if ( *list == NULL ){
        *list = pos_node_ptr;
    }
    else
        last->next = pos_node_ptr;

    last = pos_node_ptr;
    pos_node_ptr->next  = NULL;
}

void printlist(DIR* dirp, poslist *list)
{
    poslist *ptr = list;
    while ( ptr != NULL ) {
        seekdir(dirp, ptr->pos);
        printf("%s\n", readdir(dirp)->d_name);
        ptr = ptr->next;
    }
}

void eraselist(poslist **list)
{
    poslist *ptr = *list;
    poslist *next;

    while ( NULL != ptr ) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }
    *list = NULL;
}


void listdir(DIR *dirp, int flags)
{
    struct dirent  *entry;
    long int       pos;
    poslist        *saved_positions = NULL;

    while ( 1 ) {
        pos = telldir(dirp);       /* Save current position.     */
        errno = 0;                 /* Try to read entry.         */
        if ( NULL == (entry = readdir(dirp) ) && errno != 0 )
            perror("readdir");     /* Error reading entry.       */
        else if ( entry == NULL )
            break;
        else {
            if ( (flags & LIST_DIRS_FIRST) && !isdir(entry) )  {
                save(pos, &saved_positions);
                continue;
            }
            printf("%s/\n", entry->d_name);
        }
    }
    if ( flags & LIST_DIRS_FIRST )
        printlist(dirp, saved_positions);
    eraselist(&saved_positions);
}

/*****************************************************************************
                               Main Program
*****************************************************************************/
int main(int argc, char *argv[])
{
    DIR   *dirp;
    int    i;
    int    ls_flags = LIST_DIRS_FIRST;

    if ( 1 == argc ) {      /* No arguments; use current working directory. */
        errno = 0;
        if ( ( dirp = opendir(".") ) == NULL )
            fatal_error(errno, "opendir");           /* Could not open cwd. */
        else
           listdir( dirp, LIST_DIRS_FIRST );
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
                listdir( dirp, ls_flags );
                closedir(dirp);
            }
        }
    }
    return 0;
}

