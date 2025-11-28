/*****************************************************************************
  Title          : mqsend_demo1.c
  Author         : Stewart Weiss
  Created on     : June 12, 2024
  Description    : Sends messages to a specified queue
  Purpose        : To show how to use message queues
  Usage          : mqsend_demo  /<mq-name>
  Build with     : gcc -Wall -g -I../include -L../lib  -o mqsend_demo \
                    mqsend_demo.c -lrt
  Modifications  :
  11/28/2025 by SNW
  Inserted call to close message queue before the call to
  fatal_error(), and removed code after while loop.

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

int  main(int argc, char *argv[])
{
    mqd_t  mqdes;              /* The message queue descriptor      */
    struct mq_attr attr;       /* Message queue attribute structure */
    char   *msg_buffer;        /* Stores the data read from stdin.  */
    unsigned int length;       /* Length of sent message            */

    if (argc != 2) {
       fprintf(stderr, "Usage: %s <mq-name>\n", argv[0]);
       exit(EXIT_FAILURE);
    }

    if ( (mqd_t) -1 == (mqdes = mq_open(argv[1],  O_WRONLY)))
        fatal_error(errno, "mq_open");

    if (-1 == mq_getattr(mqdes, &attr) )
        fatal_error(errno, "mq_getattr");

    if ( NULL == (msg_buffer = malloc(attr.mq_msgsize)) )
        fatal_error(errno, "malloc");
    // OMITTED: Set up signal handling.
    while ( TRUE ) {
        if ( scanf("%ms", &msg_buffer) > 0 ) {
            length = strlen(msg_buffer);
            if ( length <= attr.mq_msgsize )
                mq_send(mqdes, msg_buffer, length, length);
            else {
                mq_close(mqdes);
                fatal_error(-1, "String data too long");
            }
        }
        free(msg_buffer);
    }
}

