/*****************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.lgplv3 for details.               *
*****************************************************************************/

#include "common_hdrs.h"

/** error_message()
   This prints an error message associated with errnum  on standard error
   if errnum > 0. If errnum <= 0, it prints the msg passed to it.
   It does not terminate the calling program.
   This is used when there is a way  to recover from the error.               */
void error_mssge(int errornum, const char * msg)
{
    if (errornum > 0) {
        /* an errno value */
        fprintf(stderr, "%s: %s\n", msg, strerror(errornum));
    }
    else /* project-defined error number - ignore and just print msg */
        fprintf(stderr, "%s\n", msg);
}


/** fatal_error()
   This prints an error message associated with errnum  on standard error
   before terminating   the calling program, if errnum > 0.
   If errnum <= 0, it prints the msg passed to it.
   fatal_error() should be called for a nonrecoverable error.                 */
void fatal_error(int errornum, const char * msg)
{
    error_mssge(errornum, msg);
    exit(EXIT_FAILURE);
}


/** usage_error()
   This prints a usage error message on standard output, advising the
   user of the correct way to call the program, and terminates the program.   */
void usage_error(const char * msg)
{
    fprintf(stderr,"usage: %s\n", msg);
    exit(EXIT_FAILURE);
}



