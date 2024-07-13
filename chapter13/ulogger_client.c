/*****************************************************************************
  Title          : logger_client.c
  Author         : Stewart Weiss
  Created on     : June 16, 2024
  Description    : Client program for logger.c
  Purpose        : To send simulated login data to the logger
  Usage          :
  Build with     :

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
#include <mqueue.h>
#include <sys/wait.h>
#include "ulogger.h"

int  main(int argc, char *argv[])
{
    mqd_t  mqdes;              /* The message queue descriptor      */
    struct mq_attr attr;       /* Message queue attribute structure */
    char   *msg_buffer;        /* Stores the data read from stdin.  */
    msgtype msg;               /* The message to be sent            */
    char   errstring[128];     /* For error messages                */

    if (argc != 3)
        usage_error( "mqregister nickname line\n");

    if ( strlen(argv[1]) > MAX_NAME-1 ) {
        sprintf(errstring, "Name must be less than %d characters\n", MAX_NAME);
        fatal_error(-1, errstring);
    }

    if ( strlen(argv[2]) > MAX_LINE-1) {
        sprintf(errstring, "Line must be less than %d characters\n", MAX_LINE);
        fatal_error(-1, errstring);
    }
    strcpy(msg.name, argv[1]);
    strcpy(msg.line, argv[2]);

    if ( (mqd_t) -1 == (mqdes = mq_open(mqname,  O_WRONLY)))
        fatal_error(errno, "mq_open");

    if (-1 == mq_getattr(mqdes, &attr) )
        fatal_error(errno, "mq_getattr");

    if ( NULL == (msg_buffer = malloc(attr.mq_msgsize)) )
        fatal_error(errno, "malloc");

    mq_send(mqdes, (char*) &msg, sizeof msg, 0);
    mq_close(mqdes);
    exit(EXIT_SUCCESS);
}

