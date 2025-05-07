/*****************************************************************************
  Title          : spl_utmpdump.c
  Author         : Stewart Weiss,
  Created on     : June 12, 2023
  Description    : Demonstrates how to process utmp structures
  Purpose        : To display the contents of the utmp or wtmp file
  Usage          : spl_utmpdump [wtmp]
                   if wtmp argument supplied, it prints the contents of
                   wtmp file, otherwise it prints the utmp file
  Build with     : gcc -o spl_utmpdump spl_utmpdump.c  -I../include \
                          -L../lib -lutils

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

#define _GNU_SOURCE
#include "common_hdrs.h"
#include <utmpx.h>
#include <paths.h>                /* For definition of _PATH_LASTLOG        */

/* print_header_row  prints a heading for the output.                       */
void print_header_row( )
{
    printf("%-14s%-7s%-9s%-9s%-9s","TYPE","PID","USER","ID","LINE");
#ifdef SHOW_EXIT
    printf("%-9s", "STATUS");
#else
    printf(" ");
#endif
    printf("%-19s%-16s\n","HOST","TIME");
}


/*  print_ut_type prints the string representation of the integer value
    of utmp type  */
void print_ut_type( int t)
{
    switch (t)
    {
    case RUN_LVL:      	printf("RUN_LVL       "); break;
    case BOOT_TIME:     printf("BOOT_TIME     "); break;
    case NEW_TIME:      printf("NEW_TIME      "); break;
    case OLD_TIME:      printf("OLD_TIME      "); break;
    case INIT_PROCESS:  printf("INIT_PROCESS  "); break;
    case LOGIN_PROCESS: printf("LOGIN_PROCESS "); break;
    case USER_PROCESS:  printf("USER_PROCESS  "); break;
    case DEAD_PROCESS:  printf("DEAD_PROCESS  "); break;
    case ACCOUNTING:    printf("ACCOUNTING    "); break;
    }
}


/*  print_one_rec() prints the contents of the utmp struct in human readable
    form. The sizes used in the printf below may not display well on all
    systems.
*/
void print_one_rec( struct utmpx *utbufp )
{
    struct tm * bdtime;
    char   timestring[64];

    print_ut_type(utbufp->ut_type);
    printf("%-6d ",   utbufp->ut_pid);       /* Process id */
    printf("%-8.8s ", utbufp->ut_user);      /* User name  */
    printf("%-8.8s ", utbufp->ut_id);        /* utmp id    */
    printf("%-8.8s ", utbufp->ut_line);      /* Line       */
#ifdef SHOW_EXIT
    printf("%-3d ",   utbufp->ut_exit.e_exit);
    printf("%-3d ",   utbufp->ut_exit.e_termination);
#endif
    if ( utbufp->ut_host[0] != '\0' )
        printf(" %-19s", utbufp->ut_host);   /* Host    */
    else
        printf(" %-19s", " ");

#if __WORDSIZE_TIME64_COMPAT32
    time_t utmp_time =  utbufp->ut_tv.tv_sec;
    bdtime = localtime(&utmp_time);
#else
    bdtime = localtime(&(utbufp->ut_tv.tv_sec));
#endif
    if (bdtime == NULL)
        fatal_error(EOVERFLOW, "localtime");

    /* Create a string from the broken down time using the %c format */
    if (0 == strftime(timestring, sizeof(timestring),"%c", bdtime) )
        fatal_error(-1, "Conversion to a date-time string failed "
                    " or produced an empty string\n");
    printf("%s\n", timestring);
}


/*****************************************************************************
                               Main Program
*****************************************************************************/
int main(int argc, char *argv[])
{
    struct utmpx     *utmp_entry;   /* For returned pointer from getutxent */

    if ( (argc > 1) && (strcmp(argv[1],"wtmp") == 0) ) {
        if ( -1 == utmpxname(WTMPX_FILE) )
            fatal_error(errno, "utmpxname()");
    }
    else
        if ( -1 == utmpxname(UTMPX_FILE) )
            fatal_error(errno, "utmpxname()");

    print_header_row();
    setutxent();
    errno = 0;
    while( (utmp_entry = getutxent()) != NULL )
        print_one_rec( utmp_entry );
    if ( 0 != errno )
        fatal_error(errno, "getutxent()");

    endutxent();
    return 0;
}
