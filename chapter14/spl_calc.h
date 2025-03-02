/*****************************************************************************
  Title          : calc.h
  Author         : Stewart Weiss
  Created on     : May  2, 2008
  Description    : Common header file for calc.c and calcd.c

  Notes          : The message struct contains the name of a FIFO and a
                   string to be converted to upeprcase.  The client fills in
                   the struct with the name of its private FIFO and the string
                   to be converted and sends it to the server through the
                   public FIFO.
                   The server extracts the private FIFO name and string from
                   the message, creates a new string in uppercase, and sends
                   the converted string to the client through its private
                   FIFO.

******************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/



#include "common_hdrs.h"
#include <sys/stat.h>

#define PUBLIC        "/tmp/CALCFIFO"
#define FIFOPATHLEN   (PIPE_BUF/4)
#define TEXTLEN       (PIPE_BUF - FIFOPATHLEN)

/*
   Because the message must be no larger than PIPE_BUF bytes. The sum
   FIFOPATHLEN + TEXTLEN == PIPE_BUF
*/
struct message {
    char   fifo_name[FIFOPATHLEN];
    char   text[TEXTLEN];
};




