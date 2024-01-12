/*****************************************************************************
  Title          : spl_stat.c
  Author         : Stewart Weiss
  Created on     : September 10, 2023
  Description    : Simulates the stat command, with only the -L option
  Purpose        : To interact with the filesystem API
  Usage          : spl_stat  [-L] file file ...
  Build with     : gcc -Wall -g -I ../include spl_stat.c -o spl_stat \
                   -L../lib -lspl

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



#define _GNU_SOURCE    /* Needed to expose statx() function in glibc */
#include <sys/stat.h>  /* Required according to man page for statx() */
#include <pwd.h>
#include <grp.h>
#include <sys/sysmacros.h>

#define NUM_FIELDS          13    /* Number of fields in statx structure */
#include "common_hdrs.h"

/****************************************************************************/
char* mode2str( int mode)
{
    static char str[] = "----------";           /* Initial string   */

    if      ( S_ISDIR(mode) )  str[0] = 'd';    /* Directory         */
    else if ( S_ISCHR(mode) )  str[0] = 'c';    /* Char devices      */
    else if ( S_ISBLK(mode) )  str[0] = 'b';    /* Block device      */
    else if ( S_ISLNK(mode) )  str[0] = 'l';    /* Symbolic link     */
    else if ( S_ISFIFO(mode))  str[0] = 'p';    /* Named pipe (FIFO) */
    else if ( S_ISSOCK(mode))  str[0] = 's';    /* Socket            */

    if ( mode & S_IRUSR ) str[1] = 'r';
    if ( mode & S_IWUSR ) str[2] = 'w';
    if ( mode & S_IXUSR ) str[3] = 'x';

    if ( mode & S_IRGRP ) str[4] = 'r';
    if ( mode & S_IWGRP ) str[5] = 'w';
    if ( mode & S_IXGRP ) str[6] = 'x';

    if ( mode & S_IROTH ) str[7] = 'r';
    if ( mode & S_IWOTH ) str[8] = 'w';
    if ( mode & S_IXOTH ) str[9] = 'x';

    /* Now check the setuid, setgid, and sticky bits */
    if ( mode & S_ISUID ) str[3] = 's';
    if ( mode & S_ISGID ) str[6] = 's';
    if ( mode & S_ISVTX ) str[9] = 't';
    return str;
}

/****************************************************************************/
char* uid2name ( uid_t uid )
{
    struct  passwd *pw_ptr;

    if ( ( pw_ptr = getpwuid( uid ) ) == NULL )
          return "";
    else
      return pw_ptr->pw_name ;
}

/*--------------------------------------------------------------------------*/
char* gid2name ( gid_t gid  )
{
    struct group *grp_ptr;
    static  char numstr[10];

    if ( ( grp_ptr = getgrgid(gid) ) == NULL ) {
          sprintf(numstr,"%d", gid);
          return numstr;
    }
    else
          return grp_ptr->gr_name;
}

/*--------------------------------------------------------------------------*/
void ids2hexdecstr(unsigned int major, unsigned int minor, char* buffer)
{
    sprintf(buffer, "%02x%02xh/%lud", major, minor,  makedev(major, minor));
}



/*--------------------------------------------------------------------------*/
void print_time(const char* label, struct statx_timestamp *time_field)
{
    struct tm *bdtime;                 /* Broken-down time                 */
    char       formatted_time[100];    /* String storing formatted time    */
    char       timezone[32];           /* To store time offset             */
    time_t     time_val;               /* For converted tv_sec field       */

    time_val = time_field->tv_sec;          /* Convert to time_t           */
    bdtime = localtime(&time_val);          /* Convert to broken-down time */
    if (bdtime == NULL)                     /* Check for error             */
        fatal_error(EOVERFLOW, "localtime");

    /* Create a string from the broken down time. */
    if ( strftime(formatted_time, sizeof(formatted_time), "%F %T", bdtime) == 0 )
        fatal_error(BAD_FORMAT_ERROR,"strftime failed\n");

    printf("%s%s.%09u", label, formatted_time, time_field->tv_nsec );
    if ( 0 == strftime(timezone, 32, " %z", bdtime) )
        fatal_error(BAD_FORMAT_ERROR, "Error printing time zone\n");
    printf("%s\n", timezone);
}

/*--------------------------------------------------------------------------*/
/* A function to print the statx structure using the same format as the stat
   command */
void print_statx(struct statx *stx, int what2print[])
{
    char idstring[64];

    if (stx->stx_mask & STATX_SIZE)
        printf("  Size: %-16llu", (unsigned long long)stx->stx_size);

    if (stx->stx_mask & STATX_BLOCKS)
        printf("Blocks: %-10llu", (unsigned long long)stx->stx_blocks);

    printf(" IO Block: %-7lu", (unsigned long )stx->stx_blksize);

    if (stx->stx_mask & STATX_TYPE)
        switch (stx->stx_mode & S_IFMT) {
        case S_IFIFO:printf("  FIFO\n"); break;
        case S_IFCHR:printf("  character special file\n");break;
        case S_IFDIR:printf("  directory\n"); break;
        case S_IFBLK:printf("  block special file\n");break;
        case S_IFREG:printf("  regular file\n");break;
        case S_IFLNK:printf("  symbolic link\n"); break;
        case S_IFSOCK:printf("  socket\n");     break;
        default:
            printf(" unknown type (%o)\n", stx->stx_mode & S_IFMT);
            break;
        }
    else
        printf(" no type\n");

    ids2hexdecstr(stx->stx_dev_major, stx->stx_dev_minor, idstring);
    printf("Device: %-16s", idstring);

    if (stx->stx_mask & STATX_INO)
        printf("Inode: %-11llu", (unsigned long long) stx->stx_ino);

    if (stx->stx_mask & STATX_NLINK)
        printf(" Links: %-5lu", (unsigned long) stx->stx_nlink);

    if (stx->stx_mask & STATX_TYPE) {
        switch (stx->stx_mode & S_IFMT) {
        case S_IFBLK:
        case S_IFCHR:
            printf(" Device type: %lu,%lu",
                  (unsigned long) stx->stx_rdev_major, (unsigned long) stx->stx_rdev_minor);
            break;
        }
    }
    printf("\n");

    if (stx->stx_mask & STATX_MODE)
        printf("Access: (%04o/%s)", stx->stx_mode & 07777, mode2str((int) stx->stx_mode));

    if (stx->stx_mask & STATX_UID)
        printf("  Uid: (%5ld/ %s)  ", (long) stx->stx_uid, uid2name(stx->stx_uid));

    if (stx->stx_mask & STATX_GID)
        printf("  Gid: (%5ld/ %s)\n", (long) stx->stx_gid, gid2name(stx->stx_gid));

    if (stx->stx_mask & STATX_ATIME)
        print_time("Access: ", &stx->stx_atime);

    if (stx->stx_mask & STATX_MTIME)
        print_time("Modify: ", &stx->stx_mtime);

    if (stx->stx_mask & STATX_CTIME)
        print_time("Change: ", &stx->stx_ctime);

    if (stx->stx_mask & STATX_BTIME)
        print_time(" Birth: ", &stx->stx_btime);
}

/*--------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
    struct statx statx_buffer;    /* statx structure filled by statx()        */
    char         usage_mssge[128];/* string to store usage message            */
    unsigned int mask;            /* mask to pass to statx()                  */
    char         options[] = "L"; /* String for getopt() processing           */
    int          report_on_link;  /* Flag indicating whether to report on link*/
    ssize_t      nbytes;          /* Return value of readlink()               */
    char         target[256];
    int          to_print[NUM_FIELDS];
    int          i;
    char         ch;

    /* Set the mask to request the basic fields and the birth time.
       These are what the stat command displays by default. */
    mask = STATX_BASIC_STATS | STATX_BTIME;

    /* Initialize the array of fields to print. */
    for ( i = 0; i < NUM_FIELDS; i++ ) to_print[i] = 1;

    /* Default behavior is to report on symbolic links, not their targets.    */
    report_on_link = AT_SYMLINK_NOFOLLOW; /* See the man page. */

    /* Make the program locale-sensitive so that times are printed using
       the user's locale settings.*/
    if ( setlocale(LC_TIME, "") == NULL )
        fatal_error(LOCALE_ERROR,"setlocale() could not set the given locale");

    /* Parse the command line for options. */
    while  (TRUE) {
        /* Call getopt, passing argc and argv and the options string */
        ch = getopt(argc, argv, options);
        if ( -1 == ch ) /* No more options  */
            break;
        switch ( ch ) {
        case 'L':      /* follow symbolic links and report on their targets. */
            report_on_link = 0;
            break;
        case '?' :
            fprintf(stderr,"Found invalid option %c\n", optopt);
            sprintf(usage_mssge, "%s [-L ] files ...\n", basename(argv[0]));
            usage_error(usage_mssge);
            break;
        }
    }
    /* If no file arguments, print a usage message. */
    if (optind >= argc) {
        sprintf(usage_mssge,"usage: %s [-L] files ...\n", basename(argv[0]));
        usage_error(usage_mssge);
    }
    /* For each file argument, call statx() and print its metadata. */
    for ( i = optind; i < argc; i++) {
        if ( statx(AT_FDCWD, argv[i], report_on_link, mask, &statx_buffer) < 0 )
             printf("Could not stat file %s\n", argv[i]);
        else {
            if ( S_ISLNK(statx_buffer.stx_mode)) {  /* File's a soft link */
                if ( report_on_link == AT_SYMLINK_NOFOLLOW) {
                    /* Report is of the link itself, not its target, so
                       write the file name in the form 'link -> target' */
                    errno = 0;
                    if ( -1 == (nbytes = readlink(argv[i], target, sizeof(target))))
                        error_mssge(errno, "readlink");
                    else {
                        target[nbytes] = '\0';
                        printf("  File: %s -> %s\n", argv[i], target);
                    }
                }
                else /* Report is of the target */
                    printf("  File: %s\n", argv[i]);
            }
            else
                printf("  File: %s\n", argv[i]);

            print_statx(&statx_buffer, to_print);
        }
        /* If there's another file, print a dashed separator line */
        if ( i < argc-1 )
            printf("----------------------------------"
                       "-----------------------------------------\n");
    }
    return 0;
}
