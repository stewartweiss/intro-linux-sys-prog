

#ifndef COMMON_HDRS_H
#define COMMON_HDRS_H

#include <sys/types.h>   /* type definitions used by many programs  v */
#include <stdlib.h>      /* prototypes of many C functions and macros */
#include <stdio.h>       /* C standard I/O library                    */
#include <string.h>      /* string functions                          */
#include <limits.h>      /* system limit constants                    */
#include <unistd.h>      /* prototypes of most system calls           */
#include <time.h>        /* time related functions                    */
#ifndef LINUX_FCNTL
    #include <fcntl.h>       /* file I/O related functions            */
#endif
#include <errno.h>       /* errno and error constants and functions   */
#include <locale.h>      /* For localization                          */

/* Non-system headers */
#include "get_nums.h"    /* string to number conversions              */
#include "error_exits.h" /* error-handling and exit functions         */



/* Define various constants and types used throughout the examples.   */
#define  STRING_MAX  1024

/* Create a BOOL type */
#ifdef FALSE
    #undef FALSE
#endif
#ifdef TRUE
    #undef TRUE
#endif

#ifdef BOOL
    #undef BOOL
#endif
typedef enum{FALSE, TRUE} BOOL;


#define MAXLEN  STRING_MAX        /* Maximum size of message string         */

/* These are used by locale-related programs */
#define FORMAT  "%c"              /* Default format string                  */
#define BAD_FORMAT_ERROR    -1    /* Error in format string                 */
#define TIME_ADJUST_ERROR   -2    /* Error to return if parsing problem     */
#define LOCALE_ERROR        -3    /* Non-specific error from setlocale()    */

/* General errors */
#define READ_ERROR          -4    /* Incomplete read of a file             */
#define MEM_ERROR           -5    /* Insufficient memory                   */

#endif /* COMMON_HDRS_H */
