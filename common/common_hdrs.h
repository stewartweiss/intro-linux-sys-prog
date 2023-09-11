

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


#endif /* COMMON_HDRS_H */
