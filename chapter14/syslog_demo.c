/*****************************************************************************
  Title          : syslog_demo.c
  Author         : Stewart Weiss
  Created on     : May 16, 2024
  Description    : Adds messages from user to syslog file
  Purpose        : To show how to use syslog()
  Usage          : syslog_demo
  Build with     : gcc -Wall -I../include -L../lib -g -o syslog_demo \
                          syslog_demo.c -lspl

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
#include  <syslog.h>

int main( int argc, char *argv[])
{

    char  msg[512];

    openlog(argv[0], LOG_PID | LOG_CONS, LOG_LOCAL0); /* Start syslog logging. */
    strcpy(msg, "Starting logging demonstration.");
    while ( strcmp(msg, "quit")  != 0) {
        syslog(LOG_INFO, msg);
        printf("Message to log: ");
        fflush(stdout);
        scanf("%s", msg);
    }
    exit(EXIT_SUCCESS);
}


