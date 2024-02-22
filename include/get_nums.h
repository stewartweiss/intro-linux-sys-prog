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
#ifndef GET_NUMS_H
#define GET_NUMS_H


/* flags to pass to functions */
#define NO_TRAILING           1     /* forbid trailing characters     */
#define NON_NEG_ONLY          2     /* forbid negative numbers        */
#define ONLY_DIGITS           4     /* forbid strings with no digits  */
#define PURE   NO_TRAILING | ONLY_DIGITS

/* return codes */
#define VALID_NUMBER          0     /* successful processing                 */
#define FATAL_ERROR          -1     /* ERANGE or EINVAL returned by strtol() */
#define TRAILING_CHARS_FOUND -2     /* characters found after number         */
#define OUT_OF_RANGE         -3     /* int requested but out of int range    */
#define NO_DIGITS_FOUND      -4     /* no digits in string                   */
#define NEG_NUM_FOUND        -5     /* negative number found but not allowed */

/** get_long()
    On successful processing, it returns VALID_NUMBER and stores the resulting
    number in *value, otherwise it returns one of the non-zero error codes.
    On return, msg points to a string with a suitable message for the caller
    to pass to an error-handling function.
    flags is used to decide whether trailing characters, negative values,
    and zeros for strings without any digits are allowed or should be errors.
 *  @param  char*     arg     [IN]  string to parse
 *  @param  int       flags   [IN]  flag specifying how to handle anomalies
 *  @param  long*     value   [OUT] returned long int
 *  @param  char*     msg     [OUT] if not empty, error message
 *  @return int       VALID_NUMBER or a negative error code indicating the
                      type of error
 */
int  get_long(char *arg, int flags, long *value, char *msg );

/** get_int()
    On successful processing, returns VALID_NUMBER and stores the resulting value
    in value, otherwise returns one of the  error codes..
    On return, msg contains a string with a suitable message for the caller
    to pass to an error-handling function.
    flags is used to decide whether trailing characters, negative values,
    and zeros for strings without any digits are allowed.
 *  @param  char*     arg     [IN]  string to parse
 *  @param  int       flags   [IN]  flag specifying how to handle anomalies
 *  @param  int*      value   [OUT] returned long int
 *  @param  char*     msg     [OUT] if not empty, error message
 *  @return int       VALID_NUMBER or a negative error code indicating the
                      type of error
 */
int  get_int(char *arg, int flags, int *value, char *msg );

/** get_dbl()
    On successful processing, returns VALID_NUMBER and stores the resulting value
    in value, otherwise returns one of the  error codes..
    On return, msg contains a string with a suitable message for the caller
    to pass to an error-handling function.
    flags is used to decide whether trailing characters, negative values,
    and zeros for strings without any digits are allowed.
 *  @param  char*     arg     [IN]  string to parse
 *  @param  int       flags   [IN]  flag specifying how to handle anomalies
 *  @param  double*   value   [OUT] returned double
 *  @param  char*     msg     [OUT] if not empty, error message
 *  @return int       VALID_NUMBER or a negative error code indicating the
                      type of error
 */
int  get_dbl(char *arg, int flags, double *value, char *msg );


void lltostr(long long int num, char* str, int width);

#endif
