/*****************************************************************************
  Title          : ps_utils.h
  Author         : Stewart Weiss
  Created on     : August 24, 2024
  Description    : A header file containing prototypes for proc-related tasks
  Purpose        :
  Usage          :
  Build with     :

  Notes:
  This file contains utility functions used by programs that access the
  process data in the /proc directory's stat files and other files.

******************************************************************************
* Copyright (C) 2024 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/
#ifndef _PS_UTILS_H
#define _PS_UTILS_H

#include "common_hdrs.h"
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

#define START_FORMAT "%H:%M"
#define MAX_NAME    9
#define MAX_LINE    512

typedef struct
{
    int   pid;
    int   uid;
    char* comm;
    char  state;
    int   ppid;
    int   pgrp;
    int   session;
    int   tty_nr;
    unsigned long utime;
    unsigned long stime;
    long  priority;
    long  nice;
    unsigned long long start_time;
    unsigned long vsize;
    long  rss;
    long  shared;
    double cpu_pct;
    double mem_pct;
}  procstat;


/** get_hertz() returns the number of clock ticks per second, also known
    as HZ, or the inverse of a system jiffy.
 */
long get_hertz ();

/** name2uid(n) returns the uid of the given name.
 */
int name2uid ( char * name );

/** uid2name(u) returns the username of the given uid u, truncated to
    MAX_NAME (10) characters.
 */
char* uid2name ( uid_t uid );

/** get_boot_time(btime) returns the  system boot time in *btime,
    retrieved from /proc/stat.
 */
void get_boot_time(unsigned long long *btime);

void get_cpu_time_str( procstat ps, char* cputimestr );

/** make_cpu_time_str(ps, str) computes the sum of stime and utime in the
    procstat structure ps, converts it to a long int, and formats the
    total time in a string HH:MM:SS unless it is greater than 24 hours,
    in which case it formats it as DD:HH:MM:SS
*/
void make_cpu_time_str( procstat ps, char* cputimestr );

/** make_start_time(ps, st) converts the start time, boot time + start
    time stored in procstat ps to a string in st. If it is in the same
    calendar year and day as the current time, it uses START_FORMAT, which is
    HH:MM, otherwise if in the same year but different day, MM:DD, otherwise
    just the year as YYYY.
*/
void make_start_time_str(procstat ps, char* start_time );

/** tty_name(str, major, minor) fills str with the string name for the
    tty represented by the major/minor device number pair.
    This leaves out a lot of possibilities. It's just an approximation. */
int tty_name(char *buf, unsigned maj, unsigned min);

/** parse_buf(buf, ps) fills the procstat stucture ps with the fields
   from the stat file.
*/
int parse_buf(char* buf, procstat *ps);

/** printheadings() prints the headings for the columns of the ps output
    that this program allows into buf. It isn't very extensible design.
    It should instead accept the heading strings as arguments and look up
    their formatting, but this is left as an exercise. */
void printheadings(char *buf);

/** strip_cmmd_parens(cmd) returns the string obtained by removing
    the parentheses  in the first and  last positions of cmd.
*/
char* strip_cmmd_parens(char* comm);

/** print_one_ps(ps) prints a single process's stat information into buf,
    using the format specifiers defined by the man page for proc.
*/
void print_one_ps( procstat ps, char* buf);



#endif //_PS_UTILS_H







