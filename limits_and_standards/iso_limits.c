/******************************************************************************
  Title          : iso_limits.c
  Author         : Stewart Weiss
  Created on     : January 27, 2011
  Description    : Displays the values of the ISO C static constants 
  Purpose        : To show the values of the ISO C limits on the given system.
  Usage          : iso_limits
  Build with     : gcc -o iso_limits iso_limits.c
  Modifications  :
  Notes          : The constants whose names appear in the print statements
                   below are defined in the file <limits.h> on an ISO-C99 
                   compliant implementation of UNIX. These are compile-time
                   limits. If the system is not ISO C compliant, then there
                   will be compile-time errors, because the compiler will not
                   find the given symbol.

                   The actual values that are displayed when this program is 
                   run may be larger than the values specified in the ISO C
                   standard, which specifies MINIMUMs for these values.

                   When you run this, look at the values of CHAR_MAX and 
                   CHAR_MIN. If they are the same as those of SCHAR_MAX and
                   SCHAR_MIN, then characters are signed values. If characters
                   are unsigned, then CHAR_MIN will be 0.
 
******************************************************************************/

#include <limits.h>
#include <stdio.h>

int main()
{
    printf("%-12s:%20d\n","CHAR_BIT", CHAR_BIT);
    printf("%-12s:%20d\n","CHAR_MAX", CHAR_MAX);
    printf("%-12s:%20d\n","CHAR_MIN", CHAR_MIN);
    printf("%-12s:%20d\n","SCHAR_MAX", SCHAR_MAX);
    printf("%-12s:%20d\n","SCHAR_MIN", SCHAR_MIN);
    printf("%-12s:%20u\n","UCHAR_MAX", UCHAR_MAX);
    printf("%-12s:%20d\n","INT_MAX", INT_MAX);
    printf("%-12s:%20d\n","INT_MIN", INT_MIN);
    printf("%-12s:%20u\n","UINT_MAX", UINT_MAX);
    printf("%-12s:%20d\n","SHRT_MIN", SHRT_MIN);
    printf("%-12s:%20d\n","SHRT_MAX", SHRT_MAX);
    printf("%-12s:%20u\n","USHRT_MAX", USHRT_MAX);
    printf("%-12s:%20ld\n","LONG_MAX", LONG_MAX);
    printf("%-12s:%20ld\n","LONG_MIN", LONG_MIN);
    //printf("%-12s:%20ld\n","LONG_BIT", LONG_BIT);
    printf("%-12s:%20lu\n","ULONG_MAX", ULONG_MAX);
    printf("%-12s:%20lld\n","LLONG_MAX", LLONG_MAX);
    printf("%-12s:%20lld\n","LLONG_MIN", LLONG_MIN);
    printf("%-12s:%20llu\n","ULLONG_MAX", ULLONG_MAX);
    printf("%-12s:%20d\n","MB_LEN_MAX", MB_LEN_MAX);

    printf("\n");
#ifdef FOPEN_MAX
    printf("FOPEN_MAX (max number of open files) = %d\n",FOPEN_MAX);
#else
    printf("FOPEN_MAX is not defined on the system.\n");
#endif

#ifdef TMP_MAX
    printf("TMP_MAX (max num of unique filenames generated by tmpnam()) = %d\n",
           TMP_MAX);
#else
    printf("TMP_MAX is not defined on the system.\n");
#endif

// This is a POSIX.1 name that should be equal to FOPEN_MAX:
#ifdef STREAM_MAX
    printf("STREAM_MAX (max number of open files) = %d\n",STREAM_MAX);
#else
    printf("STREAM_MAX is not defined on the system.\n");
#endif

    return 0;
   } 
