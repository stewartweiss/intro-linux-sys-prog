

#ifndef SYS_HDRS_H
#define SYS_HDRS_H

#include <sys/types.h>   /* type definitions used by many programs    */
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
#include <paths.h>       /* For definition of _PATH_LASTLOG           */
#include <locale.h>      /* For localization                          */
#include <utmpx.h>       /* For utmp and wtmp processing              */
#include <signal.h>      /* For all signal-related functions          */

#endif /* SYS_HDRS_H */
