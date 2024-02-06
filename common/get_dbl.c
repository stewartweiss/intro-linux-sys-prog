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

/*
On successful processing, returns SUCCESS and stores the resulting value
in value, otherwise returns FAILURE.
On return, msg contains a string with a suitable message for the caller
to pass to an error-handling function.
flags is used to decide whether trailing characters or negatives are allowed.

*/
int  get_dbl(char *arg, int flags, double *value, char *msg )

{
    char *endptr;
    double val;
    errno = 0;    /* To distinguish success/failure after call */
    val = strtod(arg, &endptr);

    /* Check for various possible errors */
    if (errno == ERANGE) {
        if ( msg != NULL )
            sprintf(msg, "%s\n", strerror(errno));
        return FATAL_ERROR;
    } else if ( errno == EINVAL && val != 0 ) {
          if ( msg != NULL )
              sprintf(msg, "%s\n", strerror(errno));
          return FATAL_ERROR;
    }
    /* errno == 0 or errno == EINVAL && val == 0 */

    if (endptr == arg) {
        /* the first invalid char is the first char of the string */
        if ( flags & ( ONLY_DIGITS | NO_TRAILING ) ) {
            if ( msg != NULL )
                sprintf(msg, "No digits in the string\n");
            return NO_DIGITS_FOUND;
        }
        else { /* accept a zero result */
            *value = 0;
            return VALID_NUMBER;
        }
    }
    /* endptr != arg but endptr is not at end of string */

    if (*endptr != '\0' && *endptr != ' ') {
      /* there are non-number characters following the number.
         which we can call an error or not, depending. */
        if ( flags & NO_TRAILING ) {
            if ( msg != NULL )
                sprintf(msg, "Trailing characters follow the number: \"%s\"\n",
                endptr);
            return TRAILING_CHARS_FOUND;
        }
    }

    /* If we got here, strtod() successfully parsed a number, but it might be
       negative, so check flag and report if necessary */
    if ( flags & NON_NEG_ONLY )
        if ( val < 0 ) {
            if (msg != NULL )
                sprintf(msg, "number is negative\n");
            arg = endptr;
            return NEG_NUM_FOUND;
        }

    *value = val;
    return VALID_NUMBER;
}
