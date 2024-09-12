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
#ifndef  _TOP_UTILS_H
#define  _TOP_UTILS_H
#include "common_hdrs.h"
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include "ps_utils.h"
#include <curses.h>


enum field_t {PID, USER, PR, NI, VIRT, RES, SHR, S, CPU, MEM, TIME, COMMAND};

typedef  int fieldmask;

#define F_PID      (1<<PID)
#define F_USER     (1<<USER)
#define F_PR       (1<<PR)
#define F_NI       (1<<NI)
#define F_VIRT     (1<<VIRT)
#define F_RES      (1<<RES)
#define F_SHR      (1<<SHR)
#define F_S        (1<<S)
#define F_CPU      (1<<CPU)
#define F_MEM      (1<<MEM)
#define F_TIME     (1<<TIME)
#define F_COMMAND  (1<<COMMAND)
#define  F_ALL     07777


/* For each pid, the sum of utime and stime from previous refresh */
typedef struct {
    pid_t  pid;
    long cputime;
} savedcpu;

/* A comparison function to pass to qsort() */
typedef int (*compar_t)(const void *, const void *, void*);

typedef struct {
    char*  name;        /* A name for future use                         */
    fieldmask mask;     /* A bitmask for this field                      */
    char* fmt;          /* The printf format spec for this field         */
    char* colheading;   /* The column headig for the field               */
    char* headingfmt;   /* The printf format spec for the column heading */
    int   width;        /* The field width for calculating line size     */
    compar_t  sortfunc; /* The comparison function for this field        */
}  field;


int cpu_pct_cmp(const void* a, const void* b, void* dir );
int mem_pct_cmp(const void* a, const void* b, void* dir );
int vsize_cmp(const void* a, const void* b,  void* dir );
int time_cmp(const void* a, const void* b,  void* dir );
int pid_cmp(const void* a, const void* b,  void* dir );
int user_cmp(const void* a, const void* b,  void* dir );
void sortprocs(  procstat* proclist, int numprocs, compar_t cmpfunc, BOOL increasing);


/** printheadings() prints the headings for the columns of the ps output
    that this program allows into buf. It isn't very extensible design.
    It should instead accept the heading strings as arguments and look up
    their formatting, but this is left as an exercise. */
void printtopheadings(field *fieldtab, fieldmask fmask, char *buf);

void print_one_proc( field* ftab, procstat ps, fieldmask fmask, char* buf);

#endif //_TOP_UTILS_H







