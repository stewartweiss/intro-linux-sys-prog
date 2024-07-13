/*****************************************************************************
  Title          : logger.h
  Author         : Stewart Weiss
  Created on     : June 10, 2024
  Description    : Header file for message queue logger demo
  Purpose        :

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
#ifndef _LOGGER_H
#define _LOGGER_H

#define MAX_NAME  24
#define MAX_LINE  10

/* The structure of a login message sent to the logger process */
typedef struct _msgtype {
    char name[MAX_NAME];   /* Chosen user name  */
    char line[MAX_LINE];   /* Supplied TTY line */
} msgtype;

/* Name of the message queue */
char  mqname[] = "/MQ_logger";

#endif /* _LOGGER_H */
