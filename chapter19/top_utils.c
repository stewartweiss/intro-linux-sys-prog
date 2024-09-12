/*****************************************************************************
  Title          : ps_utils.c
  Author         : Stewart Weiss
  Created on     :
  Description    :
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
#define _GNU_SOURCE
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include "top_utils.h"
#include <math.h>


/*
  The fields are ordered as follows:
  MEMBER         HEADING        FIELD FORMAT    HEADING FORMAT
  pid            PID            "%7d"           "%7s"
  uid2name(uid)  USER           "%-11s"         " %-10s"
  priority       PR             "%4ld"          "%-4s"
  nice           NI             "%4ld"          "%-4s"
  vsize/1024     VIRT           "%8lu"          "%6s"
  rss            RES            "%7ld"          "%7s"
  shared         SHR            "%7ld"          "%7s"
  state          S              "%2c"           "%2c"
  %cpu  (*)      %CPU           "%6.1f"         "%6s"
  %mem  (*)      %MEM           "%6.1f"         "%6s"
  cputime (*)    TIME+          "%10s"          "%10s"
  cmd            COMMAND         "%s"           " %s"
*/


int pid_cmp(const void* a, const void* b,  void* dir )
{
    if ( *((BOOL*) dir) )
        return ( ((procstat*) a)->pid - ((procstat*) b)->pid);
    else
        return ( ((procstat*) b)->pid - ((procstat*) a)->pid);
}

int user_cmp(const void* a, const void* b,  void* dir )
{
    char  name_a[11],  name_b[11];
    strcpy(name_a, uid2name(((procstat*) a)->uid));
    strcpy(name_b, uid2name(((procstat*) b)->uid));
    if ( *((BOOL*) dir) )
        return strcmp(name_a, name_b);
    else
        return strcmp(name_b, name_a);
}

int cpu_pct_cmp(const void* a, const void* b, void* dir )
{
    if ( *((BOOL*) dir) )
        return ( ((procstat*) a)->cpu_pct > ((procstat*) b)->cpu_pct);
    else
        return ( ((procstat*) b)->cpu_pct > ((procstat*) a)->cpu_pct);
}

int mem_pct_cmp(const void* a, const void* b, void* dir )
{
    if ( *((BOOL*) dir) )
        return ( ((procstat*) a)->mem_pct > ((procstat*) b)->mem_pct);
    else
        return ( ((procstat*) b)->mem_pct > ((procstat*) a)->mem_pct);
}

int vsize_cmp(const void* a, const void* b,  void* dir )
{
    if ( *((BOOL*) dir) )
        return ( ((procstat*) a)->vsize - ((procstat*) b)->vsize);
    else
        return ( ((procstat*) b)->vsize - ((procstat*) a)->vsize);
}

int time_cmp(const void* a, const void* b,  void* dir )
{
    if ( *((BOOL*) dir) )
        return ( ((procstat*) a)->utime + ((procstat*) a)->stime  >
                 ((procstat*) b)->utime + ((procstat*) b)->stime );
    else
        return ( ((procstat*) b)->utime + ((procstat*) b)->stime  >
                 ((procstat*) a)->utime + ((procstat*) a)->stime );
}

void sortprocs(  procstat* proclist, int numprocs, compar_t cmpfunc, BOOL increasing)
{
    qsort_r(proclist, numprocs, sizeof(procstat), cmpfunc, (void*)   &increasing);
}





void printtopheadings(field *ftab, fieldmask fmask, char *buf)
{
    memset(buf, 0, MAX_LINE);
    for ( int i  = PID; i <= COMMAND; i++ ) {
        if ( fmask & ftab[i].mask )
            sprintf(buf+ strlen(buf), ftab[i].headingfmt, ftab[i].colheading);
    }
}



char* prioritystr( long priority )
{
    static char  str[8];
    if ( priority > -100 ) {
        sprintf(str, "%3ld", priority);
        return str;
    }
    else
        return "rt";
}

char* vsizestr( long vsize )
{
    static char  str[10];
    if ( vsize > 1048576 )
        sprintf(str, "%7.2fg", (1.0*vsize)/1048576);
    else
        sprintf(str, "%8ld", vsize );
    return str;
}


void print_one_proc( field* ftab, procstat ps, fieldmask fmask, char* buf)
{
    char   cputimestr[16];
    char  *cmd;

    get_cpu_time_str(ps, cputimestr); /* Function from spl_ps.c    */
    cmd = strip_cmmd_parens(ps.comm); /* Function from spl_ps.c    */
    for ( int i  = PID; i <= COMMAND; i++ ) { /* For each field... */
        if ( fmask & ftab[i].mask )   /* Is field to be printed?   */
            switch ( i ) {            /* Which field?              */
            case PID:
                sprintf(buf+ strlen(buf), ftab[i].fmt, ps.pid); break;
            case USER:
                sprintf(buf+strlen(buf),  ftab[i].fmt, uid2name(ps.uid)); break;
            case PR:
                sprintf(buf+strlen(buf), ftab[i].fmt, prioritystr(ps.priority)); break;
            case NI:
                sprintf(buf+strlen(buf), ftab[i].fmt, ps.nice); break;
            case VIRT:
                sprintf(buf+strlen(buf), ftab[i].fmt, vsizestr(ps.vsize)); break;
            case RES:
                sprintf(buf+strlen(buf), ftab[i].fmt, ps.rss); break;
            case SHR:
                sprintf(buf+strlen(buf), ftab[i].fmt, ps.shared); break;
            case S:
                sprintf(buf+strlen(buf), ftab[i].fmt, ps.state); break;
            case CPU:
                sprintf(buf+strlen(buf), ftab[i].fmt, ps.cpu_pct); break;
            case MEM:
                sprintf(buf+strlen(buf), ftab[i].fmt, ps.mem_pct); break;
            case TIME:
                sprintf(buf+strlen(buf), ftab[i].fmt, cputimestr); break;
            case COMMAND:
                sprintf(buf+strlen(buf), ftab[i].fmt, cmd); break;
           }
    }
}
