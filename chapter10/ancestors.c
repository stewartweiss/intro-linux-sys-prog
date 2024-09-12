/*****************************************************************************
  Title          : ancestors.c
  Author         : Stewart Weiss
  Created on     : March 26, 2024
  Description    : Prints the process IDs of the ancestors of a process
  Purpose        : To show how to access /proc files
  Usage          : ancestors <pid>
  Build with     : gcc -g -Wall -o ancestors -I../include -L../lib \
                       ancestors.c -lspl

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
#include "get_nums.h"
#include <ctype.h>

pid_t getparentid( pid_t p)
{
    pid_t   parentpid = 0;  /* The parent PID found by the function   */
    char*   buf = NULL;     /* The line read by getline()             */
    char    pathname[32];   /* Pathname to file to open               */
    size_t  len = 512;      /* Length of line getline() returned      */
    ssize_t nbytes= 0;      /* Bytes read by getline()                */
    FILE*   fp;             /* File stream to read                    */

    memset(pathname, '\0', 32);
    sprintf(pathname, "/proc/%d/status",p);
    if ( NULL == (fp = fopen(pathname, "r") ))
        fatal_error(errno, "fopen");

    if ( NULL == (buf = malloc(len) ) )
        fatal_error(errno, "malloc");
    while ( TRUE ) {
        if ( -1 == (nbytes = getline(&buf,&len, fp ) ) )
            fatal_error(errno, "getline()");
        else if ( 1 == sscanf(buf,"PPid: %d",  &parentpid) )
            break;
        if ( 0 == nbytes )
            break;
    }
    free(buf);
    return parentpid;
}

int main( int argc, char* argv[])
{
    pid_t pid, parentpid;
    char errmessage[128];

    if ( argc > 1 ) {
        if ( VALID_NUMBER != get_int(argv[1], 0, &pid, errmessage ) )
            usage_error("bad number");
    }
    else
        pid = getpid();
    while (    ( parentpid = getparentid(pid)) > 0 ) {
        printf("%d\n", parentpid);
        pid = parentpid;
    }
    return 0;
}


