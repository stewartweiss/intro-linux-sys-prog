/*****************************************************************************
  Title          : mqrcv_demo1.c
  Author         : Stewart Weiss
  Created on     : June  12, 2024
  Description    : Displays received messages and their priorities
  Purpose        : To show how to use message queues
  Usage          : mqrcv_demo  /<mq-name
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
//#include <pthread.h>
#include <mqueue.h>
#include <sys/wait.h>

char*  mqname;

void cleanup(int signo )
{
    mq_unlink(mqname);
    exit(EXIT_SUCCESS);
}

int  main(int argc, char *argv[])
{
    mqd_t  mqdes;              /* The message queue descriptor      */
    struct mq_attr attr;       /* Message queue attribute structure */
    char   *msg_buffer;        /* Stores the received message       */
    ssize_t msg_size;          /* Size of buffer                    */
    unsigned int priority;     /* Priority of received message      */

    if ( argc != 2 ) {
        fprintf(stderr, "Usage: %s <mq-name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    mqname = argv[1];                 /* Get name of message queue to open. */
    if ( (mqd_t) -1 == (mqdes = mq_open(mqname, O_CREAT |  O_RDONLY, 0660, NULL )))
        fatal_error(errno, "mq_open");

    if (-1 == mq_getattr(mqdes, &attr) )                 /* Get attributes. */
        fatal_error(errno, "mq_getattr");

    if ( NULL == (msg_buffer = malloc(attr.mq_msgsize)) )
        fatal_error(errno, "malloc");

    sleep(20);
    while ( TRUE ) {
        memset(msg_buffer, 0, attr.mq_msgsize);
        msg_size = mq_receive(mqdes, msg_buffer, attr.mq_msgsize, &priority);
        if ( msg_size != -1 ) {
            printf("Message (priority=%d):  %s\n", priority, msg_buffer);
        }
    }
    free(msg_buffer);                     /* Free the buffer. */
    if ( -1 == mq_unlink(mqname) )        /* Mark queue for destruction. */
        fatal_error(errno, "mq_unlink");
    exit(EXIT_SUCCESS);

}

