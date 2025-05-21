/*****************************************************************************
  Title          : upcase.h
  Author         : Stewart Weiss
  Created on     : Oct. 18, 2024
  Description    : Common header file for upcased.c/ upcase.c
  Notes          : The message struct contains the names of two private FIFOs -
                   one that the client reads and one that it writes. The one
                   named converted_text_fifo is the one it reads.

*****************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/
#ifndef _UPCASE_H
#define  _UPCASE_H

#include "common_hdrs.h"
#define PUBLIC        "/tmp/UPCASE_FIFO"
#define LOCALELENGTH  128
#define NAMELENGTH   (PIPE_BUF - LOCALELENGTH)/2


/*
 Because the message must be no larger than PIPE_BUF bytes, and because we
 should be flexible enough to allow FIFO pathnames of a large size, the
 struct is split equally between the length of the two FIFO names.
*/
typedef struct  _message {
    char   upcased_fifo [NAMELENGTH];
    char   raw_text_fifo[NAMELENGTH];
    char   locale[LOCALELENGTH];
} message;

#endif //  _UPCASE_H
