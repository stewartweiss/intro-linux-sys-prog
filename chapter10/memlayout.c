/*****************************************************************************
  Title          : memlayout.c
  Author         : Stewart Weiss
  Created on     : March 19, 2024
  Description/   : Displays virtual address of loaded libraries and more
  Purpose        : To show where various types of symbols are in virtual mem
  Usage          : memlayout
  Build with     : gcc -Wall -g -o memlayout -I../include  memlayout.c \
                     -L../lib -lspl

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


extern char etext, edata, end;

int main(int argc, char *argv[])
{
    char       *line = NULL;
    size_t      len = 0;
    ssize_t     nread;
    FILE*       fp;

    if ( (fp = fopen("/proc/self/maps", "r")) == NULL)
        fatal_error(errno, "fopen");

    while ((nread = getline(&line, &len, fp)) != -1)
        fwrite(line, nread, 1, stdout);

    free(line);
    fclose(fp);
    exit(EXIT_SUCCESS);
}



