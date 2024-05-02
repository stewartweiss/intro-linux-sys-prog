/*****************************************************************************
  Title          :
  Author         : Stewart Weiss
  Created on     :
  Description    :
  Purpose        :
  Usage          :
  Build with     :

******************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/
#include "common_hdrs.h"
#include <dirent.h>
#include <pwd.h>
#include <sys/sysmacros.h>

#define START_FORMAT "%H:%M"
#define MAX_NAME    10
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
}  procstat;

static unsigned long long seconds_since_epoch;
static long hz;

/** get_hertz() returns the number of clock ticks per second, also known
    as HZ, or the inverse of a system jiffy.
 */
long get_hertz ()
{
#ifdef _SC_CLK_TCK  /* If this is defined prefer it. */
    if ((hz = sysconf(_SC_CLK_TCK)) > 0)
        return hz;
#endif
#ifdef HZ           /* If this is defined use it.    */
    return(HZ);
#endif
    return 100;     /* Hopefully we never need this. */
}

/** uid2name() returns the username of the given uid, truncated to
    MAX_NAME (10) characters.
 */
char* uid2name ( uid_t uid )
{
    static char name[11];  /* Persistent memory */
    struct  passwd *pw_ptr;
    if ( ( pw_ptr = getpwuid( uid ) ) == NULL )
          return "";
    else
      return strncpy(name, pw_ptr->pw_name, MAX_NAME) ;
}

/** get_boot_time() returns the  system boot time,
    retrieved from /proc/stat.
 */
void get_boot_time(unsigned long long *btime)
{
    char *buf;              /* Storage for call to getline()              */
    char *bootline  = NULL; /* Pointer to line in file with btime entry   */
    size_t  len = MAX_LINE; /* Set to zero so getline() allocates buf.    */
    FILE*  fp;

    *btime = 0;  /* In case we fail to get it. */
    if ( NULL == (fp = fopen("/proc/stat", "r")) )
        return;

    if ( NULL == (buf = malloc(MAX_LINE)))
        fatal_error(errno, "malloc");
    do {
        if ( -1 == getline(&buf,&len, fp ) )
            break;

        if ((bootline = strstr(buf, "btime ")))
            sscanf(bootline, "btime %llu", btime);
    } while ( bootline == NULL );
    free (buf);
}

void make_cpu_time_str( procstat ps, char* cputimestr )
{

    long cputime = (ps.stime + ps.utime)/hz;
    int minutes = cputime/60;
    int hours   = minutes/60;
    int seconds  = cputime%60;
    sprintf( cputimestr, "%02d:%02d:%02d",hours, minutes, seconds);
}

void make_start_time_str(procstat ps, char* start_time )
{
    unsigned long long  start;
    struct tm          *bdtime;
    struct tm          *current_time;
    struct tm           saved_start_time;
    const char* fmt =   START_FORMAT;
    unsigned long  long boot_time;

    get_boot_time(&boot_time);
    if ( 0 == boot_time)
        fatal_error(-1, "Could not get boot time");
    start = boot_time + ps.start_time/hz;
    bdtime = localtime((time_t*) (&start));
    saved_start_time = *bdtime;
    current_time = localtime((time_t*) (&seconds_since_epoch));
    if  (saved_start_time.tm_yday != current_time->tm_yday) fmt = "%b%d";
    if  (saved_start_time.tm_year != current_time->tm_year) fmt = "%Y";
    strftime(start_time, 8, fmt, &saved_start_time);
}


/** tty_name(str, major, minor) fills str with the string name for the
    tty represented by the major/minor device number pair.
    This leaves out a lot of possibilities. It's just an approximation. */
int tty_name(char *buf, unsigned maj, unsigned min)
{

    switch(maj){
      case   4:
          if (min < 64) {
              sprintf(buf, "tty%d", min);
              break;
          }
          sprintf(buf, "ttyS%d", min-64);      break;
      case  11:  sprintf(buf, "ttyB%d",  min); break;
      case  17:  sprintf(buf, "ttyH%d",  min); break;
      case  19:  sprintf(buf, "ttyC%d",  min); break;
      case  22:  sprintf(buf, "ttyD%d",  min); break; /* devices.txt */
      case  23:  sprintf(buf, "ttyD%d",  min); break; /* driver code */
      case  24:  sprintf(buf, "ttyE%d",  min); break;
      case  32:  sprintf(buf, "ttyX%d",  min); break;
      case  43:  sprintf(buf, "ttyI%d",  min); break;
      case  46:  sprintf(buf, "ttyR%d",  min); break;
      case  48:  sprintf(buf, "ttyL%d",  min); break;
      case  57:  sprintf(buf, "ttyP%d",  min); break;
      case  71:  sprintf(buf, "ttyF%d",  min); break;
      case  75:  sprintf(buf, "ttyW%d",  min); break;
      case  78:  sprintf(buf, "ttyM%d",  min); break;
      case 105:  sprintf(buf, "ttyV%d",  min); break;
      case 112:  sprintf(buf, "ttyM%d",  min); break;
      case 136 ... 143: /* A GNU extension to C */
                 sprintf(buf, "pts/%d",  min+(maj-136)*256); break;
      case 148:  sprintf(buf, "ttyT%d",  min); break;
      case 154:  sprintf(buf, "ttySR%d", min); break;
      case 156:  sprintf(buf, "ttySR%d", min+256); break;
      case 164:  sprintf(buf, "ttyCH%d",  min); break;
      case 166:  sprintf(buf, "ttyACM%d", min); break;
      case 172:  sprintf(buf, "ttyMX%d",  min); break;
      case 174:  sprintf(buf, "ttySI%d",  min); break;
      case 188:  sprintf(buf, "ttyUSB%d", min); break;
      default: return 0;
    }
    return 1;
}

/** parse_buf(buf, ps) fills the procstat stucture ps with the fields
   from the stat file.
*/
void parse_buf(char* buf, procstat *ps)
{
    sscanf(buf, " %d %ms %c %d  %d " /* pid, comm, state, ppid, pgrp        */
                " %d %d "            /* session, tty_nr                     */
                " %*d %*u %*u "      /* skipping tty_pgrp, flags, min_flt   */
                " %*u  %*u %*u "     /* skipping cmin_flt, maj_flt, cmaj_flt*/
                " %lu %lu "          /* utime, stime,                       */
                " %*d %*d "          /* skipping cutime, cstime             */
                " %ld %ld "          /* priority, nice                      */
                " %*d %*d "          /* skipping num_threads, alarm         */
                " %llu %lu",         /* start_time, vsize                   */
                                      /* skipping everything after vsize    */
            &ps->pid,
            &ps->comm,
            &ps->state,
            &ps->ppid,
            &ps->pgrp,
            &ps->session,
            &ps->tty_nr,
            &ps->utime,
            &ps->stime,
            &ps->priority,
            &ps->nice,
            &ps->start_time,
            &ps->vsize
          );
}

/** printheadings() prints the headings for the columns of the ps output
    that this program allows. It isn't very extensible design.
    It should instead accept the heading strings as arguments and look up
    their formatting, but this is left as an exercise. */
void printheadings()
{
    printf("%-11s  %-4s %6s  %-2c%4s  "
           "%4s  %6s%5s    %5s    %10s    %-10s\n",
           "UID", "PID", "PPID", 'S', "PRI",
           "NI", "STIME", "TTY", "TIME", "SIZE", "CMD");
}

char* strip_cmmd_parens(char* comm)
{
    int i = 0;

    while ( comm[i] != ')' ) i++;
    comm[i] = '\0';
    return comm+1;
}

/** print_one_ps(ps) prints a single process's stat information on a single
   line, using the format specifiers defined by the man page for proc.
*/
void print_one_ps( procstat ps)
{
    char   start_time[10];
    char   ttyname[10];
    char   cputimestr[10];
    char  *cmd;
    make_start_time_str(ps, start_time ); /* Create the start time string. */

    /* Use tty_nr field to create a name for the tty. If it returns 0,
        set the name to "?". */
    if ( ! tty_name(ttyname, major(ps.tty_nr), minor(ps.tty_nr)) )
        strcpy(ttyname, "?");

    /* Create a time string for the total cpu time (user + sys time). */
    make_cpu_time_str(ps, cputimestr);

    cmd = strip_cmmd_parens(ps.comm);

    printf("%-11s%5d%8d%3c %4ld  %4ld   %s  %-6s  %8s%10ld    %s \n",
        uid2name(ps.uid), ps.pid, ps.ppid, ps.state, ps.priority, ps.nice,
        start_time, ttyname, cputimestr,    ps.vsize/1024, cmd);
    free(ps.comm);
}

void printallprocs( DIR *dirp )
{
    struct dirent   *direntp;     /* Pointer to directory entry structure  */
    char*   accepts="0123456789"; /* For matching directory names           */
    char    pathname[PATH_MAX];   /* Pathname to file to open               */
    size_t  len = MAX_LINE;       /* Length of line getline() returned      */
    FILE*   fp;                   /* File stream to read                    */
    char*   buf;
    procstat  ps_fields;
    struct  stat  statbuffer;

    printheadings();
    if ( NULL == (buf = malloc(MAX_LINE))) /* Allocate buffer for getline() */
        fatal_error(errno, "malloc");

    while ( TRUE ) {
        errno = 0;
        direntp = readdir( dirp );                       /* Get next entry. */
        if ( direntp == NULL && errno != 0 )
            perror("readdir"); /* Not end of stream, but a readdir() error  */
        else if ( direntp == NULL )   /* The end of the stream was reached  */
            break;
        else if (strspn(direntp->d_name, accepts) == strlen(direntp->d_name))
        {   /* Directory name is a number. */
            memset(pathname, '\0', PATH_MAX);
            sprintf(pathname, "/proc/%s/stat",direntp->d_name);
            if ( NULL == (fp = fopen(pathname, "r") ))
                fatal_error(errno, "fopen");     /* Error opening stat file */
            if ( -1 == fstat(fileno(fp), &statbuffer))
                fatal_error(errno, "fstat");    /* Error stat-ing stat file */
            ps_fields.uid = statbuffer.st_uid;
            if ( -1 == getline(&buf,&len, fp ) )
                fatal_error(errno, "getline()");
            parse_buf(buf, &ps_fields);
            print_one_ps(ps_fields);
        }
    }
    free(buf);
    printf("\n");
}


int main(int argc, char* argv[])
{
    DIR   *dirp;

    seconds_since_epoch = time(NULL);
    hz = get_hertz();
    errno = 0;
    if ( ( dirp = opendir("/proc") ) == NULL )
        fatal_error(errno, "opendir");           /* Could not open cwd. */
    else
        printallprocs(dirp);
    exit(EXIT_SUCCESS);
}
