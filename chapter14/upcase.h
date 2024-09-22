/******************************************************************************
  Title          : upcase.h
  Author         : Stewart Weiss
  Created on     : May  2, 2008
  Description    : Common header file for upcased.c/ upcase.c

  Notes          : The message struct contains the names of two private FIFOs -
                   one that the client reads and one that it writes. The one
                   named converted_text_fifo is the one it reads.

******************************************************************************/
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
