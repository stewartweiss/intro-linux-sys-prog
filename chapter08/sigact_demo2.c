/*****************************************************************************
  Title       : sigact_demo2.c
  Author      : Stewart Weiss
  Created on  : December 9, 2023
  Description : Displays siginfo information about SIGFPE sent to it
  Purpose     : To show how to use siginfo_t info data
  Usage       : sigact_demo2  [f]
                The f argument causes a floating point error; without it,
                it will have a divide-by-zero error.
  Build with  : gcc -O0 -o sigact_demo2 -I../include -L../lib sigact_demo2.c \
                     -lm  -lspl


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

#define _GNU_SOURCE
#include "common_hdrs.h"
#include <signal.h>
#include <math.h>
#include <fenv.h>

BOOL noreturn = FALSE;

void fpe_handler (int signo, siginfo_t *info, void *context)
{
    printf ("Signal: %s\n", strsignal(info->si_signo));      /* UNSAFE */
    switch ( info->si_code ) {
        case FPE_INTDIV :
        printf("Code: FPE_INTDIV (Integer divide by zero)\n");
        break;

        case FPE_INTOVF :
        printf("Code: FPE_INTOVF (Integer overflow)\n");
        break;

        case FPE_FLTDIV :
        printf("Code: FPE_FLTDIV (Floating-point divide by zero)\n");
        break;

        case FPE_FLTOVF :
        printf("Code: FPE_FLTOVF (Floating-point overflow)\n");
        break;

        case FPE_FLTUND :
        printf("Code: FPE_FLTUND (Floating-point underflow)\n");
        break;

        case FPE_FLTRES :
        printf("Code: FPE_FLTRES (Floating-point inexact result)\n");
        break;

        case FPE_FLTINV :
        printf("Code: FPE_FLTINV (Floating-point invalid operation)\n");
        break;

        case FPE_FLTSUB :
        printf("Code: FPE_FLTSUB (Subscript out of range)\n");
        break;
    }
    if ( ! noreturn )
        raise(SIGTERM); /* To force program to quit. */
}

int main(int argc, char* argv[])
{
    struct sigaction action;
    float      y=2.0, z = 0.0;
    BOOL  float_divzero = FALSE;
    BOOL  float_overflow = FALSE;
    signed int  n=1,m=2;

    if ( 2 == argc )
        switch (argv[1][0]) {
        case 'f' : float_divzero  = TRUE; break;
        case 'o' : float_overflow = TRUE; break;
        case '0' : noreturn       = TRUE; break;
        }

    action.sa_sigaction = fpe_handler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&(action.sa_mask));
    int excepts = FE_DIVBYZERO|FE_INEXACT|FE_INVALID|FE_OVERFLOW|FE_UNDERFLOW;
    feenableexcept(excepts);
    if ( sigaction(SIGFPE, &action, NULL) == -1 ) {
        perror("sigaction");
        exit(1);
    }


    m = 2*n - m;
    if ( float_divzero )
        n = (int) y/z;
    else if ( float_overflow )
        feraiseexcept(FE_OVERFLOW);
    else
        n = n/m;

    return n;
}
