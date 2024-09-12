/*****************************************************************************
  Title          : spl_top.c
  Author         : Stewart Weiss
  Created on     : August 23, 2024
  Description    : A simplified top command
  Purpose        : To show how to use curses for an interactive command
  Usage          : spl_top
  Build with     : gcc -g -Wall -I../include -L../lib -o spl_top spl_top.c \
                      -lm -lspl 

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
/*
Design of spl_top.c

In order to allow columns to be added or removed, the format strings of
headings and rows must be synchronized, and each column's format must be
a separate string, stored in a variable. The print statement's format is
a concatenation of the individual format strings.

For sorting by any procstat field, an array of comparison functions is used.


*/
#define _BSD_SOURCE
#define _GNU_SOURCE
#include "common_hdrs.h"
#include <ncurses.h>
#include <math.h>
#include "top_utils.h"
#include "hash.h"


#define   SUMMARY_HEIGHT  6
#define   MAX(a,b)   ((a) >= (b))?(a):(b)
#define   MIN(a,b)   ((a) <= (b))?(a):(b)
FILE*     logfp;     // Not currently used

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


field fieldtab[] = {
     {"pid",      F_PID,     "%7d",   "PID",     "%7s",    7, pid_cmp},
     {"user",     F_USER,    " %-9s", "USER",    " %-10s",11, user_cmp},
     {"priority", F_PR,      "%3s",   "PR",      "%-4s",   4, NULL},
     {"nice",     F_NI,      "%4ld",  "NI",      "%-4s",   4, NULL},
     {"vsize",    F_VIRT,    "%8s",   "VIRT",    "%6s",    8, vsize_cmp},
     {"rss",      F_RES,     "%7ld",  "RES",     "%7s",    8, NULL},
     {"shared",   F_SHR,     "%7ld",  "SHR",     "%7s",    8, NULL},
     {"state",    F_S,       "%2c",   "S",       "%2s",    2, NULL},
     {"cpu_pct",  F_CPU,     "%6.1f", "%CPU",    "%6s",    6, cpu_pct_cmp},
     {"mem_pct",  F_MEM,     "%6.1f", "%MEM",    "%6s",    6, mem_pct_cmp},
     {"cputime",  F_TIME,    "%10s",  "TIME+",   "%10s",  10, time_cmp},
     {"cmd",      F_COMMAND, " %s",   "COMMAND", " %s",    8, NULL}
};


static volatile sig_atomic_t  caught_signal = 0;
static volatile sig_atomic_t  sigcaught;
static int  offset;    /* The offset relative to the start of a line at which
                           the substring to be printed begins. */
static int  delaysecs; /* Number of seconds between refreshes */
static int  ticks;     /* Number of ticks in a seconds */

unsigned long * prevcputime;  /* CPU time in previous refresh */
bool   show_err = TRUE;

/** cleanup_exit() called foro abnormal terminations */
void cleanup_exit(int errcode, char* message)
{
    endwin();
    fatal_error(errcode, message);
}

/** get_curtime() gets the current time in the user's locale. */
void get_curtime( char* timenow)
{
    time_t nsecs = time(NULL);
    struct tm *bdtime;

    bdtime = localtime((time_t*) (&nsecs));
    strftime(timenow, 10, "%T", bdtime);
}

/* get_uptime() stores the total uptime of the host formatted as a
   string, either S sec, M min, HH:MM, or D:HH:MM */ 
void get_uptime(char *uptime)
{
    FILE *fp;
    float  ut_secs;
    int   nsecs;

    if ( NULL == (fp = fopen("/proc/uptime", "r"))  )
        sprintf(uptime, "uptime unknown, ");
    else if ( fscanf(fp, "%f ", &ut_secs) < 1 )
        sprintf(uptime, "uptime unknown, ");
    else {
        nsecs = (int) floor((double) ut_secs);
        if ( nsecs < 60 )
            sprintf(uptime, " up  %d sec, ", nsecs);
        else if ( nsecs < 3600 )
            sprintf(uptime, " up  %d min, ", nsecs/60);
        else if ( nsecs < 86400 ) {
            int mins = (nsecs / 60) % 60;
            int hours   = nsecs / 3600;
            sprintf(uptime, " up  %d:%02d, ", hours, mins );
        }
        else {
            int mins = (nsecs / 60) % 60;
            int hours   = nsecs / 3600;
            int days = nsecs/86400;
            sprintf(uptime, " up %d+%02d:%02d, ", days, hours, mins);
        }
    }
    fclose(fp);
}


int  get_numusers()
{
    struct utmpx    utbuf;          /* read info into here */
    int             utmpfd;         /* read from this descriptor */
    struct passwd  *psswd_struct;
    uid_t          userid;
    hash_table     users;
    int            count = 0;

    init_hash(&users, 512);

    /* Open the utmpx file, check for errors. */
    if ( -1 == (utmpfd = open(UTMPX_FILE, O_RDONLY) ) )
        return -1;

    while( read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf) )
        if ( utbuf.ut_type == USER_PROCESS ) {
            psswd_struct = getpwnam(utbuf.ut_user);
            userid = psswd_struct->pw_uid;
            if ( ! is_in_hash(users, userid) ) {
                insert_hash(&users, userid);
                count++;
            }
        }
    free_hash(&users);
    close(utmpfd);
    return count;
}



void get_loadavges( char *loadstr )
{
    FILE *fp;
    float  avg1, avg5, avg15;

    if ( NULL == (fp = fopen("/proc/loadavg", "r"))  )
        sprintf(loadstr, " load averages unknown ");
    else if ( fscanf(fp, "%f %f %f", &avg1, &avg5, &avg15) < 3 )
        sprintf(loadstr, " load averages unknown ");
    else
        sprintf(loadstr, " load average: %2.2f, %2.2f, %2.2f", avg1, avg5, avg15);
    fclose(fp);

}

int get_cpustates( int *states )
{
    FILE *fp;
    int count = 0;

    if ( NULL == (fp = fopen("/proc/stat", "r"))  )
        return -1;
    fscanf(fp, "cpu ");
    for ( int i = 0; i < 8; i++ )
        count += fscanf(fp, "%d ", &(states[i]));
    fclose(fp);
    return count;
}


/** get_procmem_usage(p, vm, res, shr) fills vm, res, and shr with the virtual
    memory, resident memory and shared memory used by process p.
    If successful, it returns 1, else 0.
*/
int get_procmem_usage( pid_t p, unsigned long *vmem, long *res, long *shr)
{
    char*   buf = NULL;     /* The line read by getline()             */
    char    pathname[32];   /* Pathname to file to open               */
    size_t  len = 512;      /* Length of line getline() returned      */
    ssize_t nbytes= 0;      /* Bytes read by getline()                */
    FILE*   fp;             /* File stream to read                    */
    long    temp;
    int     count = 0;

    if ( NULL == (buf = malloc(512)) )
        return 0;
    memset(pathname, '\0', 32);
    sprintf(pathname, "/proc/%d/status",p);
    if ( NULL == (fp = fopen(pathname, "r") ))
        return 0;

    *vmem = *res = *shr = 0;
    while ( count < 4 ) {
        if ( -1 == (nbytes = getline(&buf, &len, fp ) ) ) {
            if ( 0 != errno )
                return 0;
            else
                break;
        }
        else if ( 1 == sscanf(buf,"VmSize: %ld", &temp) ) {
            *vmem = temp; count++;
        }
        else if ( 1 == sscanf(buf,"VmRSS: %ld", &temp) ) {
            *res = temp; count++;
        }
        else if ( 1 == sscanf(buf,"RssFile: %ld", &temp) ) {
            *shr += temp; count++;
        }
        else if ( 1 == sscanf(buf,"RssShmem: %ld", &temp) ) {
            *shr += temp; count++;
        }
        if ( 0 == nbytes )
            break;
    }
    free(buf);
    fclose(fp);
    return 1;
}

int get_mem_summary( char* line1, char* line2 )
{

    FILE*    fp;
    long     temp;
    double    memtotal, memfree, memused, bufcache;
    double    swaptotal, swapfree, swapused, memavail;
    int count = 0;
    int nbytes;
    char      *buf;
    size_t       len = 512;

    if ( NULL == (buf = calloc(512, 1)))
        return -1;

    if ( NULL == (fp = fopen("/proc/meminfo", "r") ))
        return -1;
    while ( count < 7 ) {
        if ( -1 == (nbytes = getline(&buf, &len, fp ) ) )
            if ( 0 != errno )
                cleanup_exit(errno, "getline()");
            else
                break;
        else if ( 1 == sscanf(buf,"MemTotal: %lu", &temp) ) {
            memtotal = ((double) temp) /1024; count++;
        }
        else if ( 1 == sscanf(buf,"MemFree: %lu", &temp) ) {
            memfree = ((double) temp) /1024; count++;
        }
        else if ( 1 == sscanf(buf,"MemAvailable: %lu", &temp) ) {
            memavail = ((double) temp) /1024; count++;
        }
        else if ( 1 == sscanf(buf,"Buffers: %lu", &temp) ) {
            bufcache = ((double) temp) /1024; count++;
        }
        else if ( 1 == sscanf(buf,"Cached: %lu", &temp) ) {
            bufcache += ((double) temp) /1024;count++;
        }
        else if ( 1 == sscanf(buf,"SwapTotal: %lu", &temp) ) {
            swaptotal = ((double) temp) /1024; count++;
        }
        else if ( 1 == sscanf(buf,"SwapFree: %lu", &temp) ) {
            swapfree = ((double) temp) /1024; count++;
        }
        if ( 0 == nbytes )
            break;
    }
    memused = memtotal - memavail;
    swapused = swaptotal - swapfree;

    sprintf(line1, "MiB Mem : %7.1f total, %7.1f free, %7.1f used,"
                        "%7.1f buff/cache,",
                    memtotal, memfree, memused, bufcache);
    sprintf(line2, "MiB Swap: %7.1f total, %7.1f free, %7.1f used, "
                        "%7.1f avail Mem",
                    swaptotal, swapfree, swapused, memavail);
    fclose(fp);
    free(buf);
    return 1;
}




void calc_memsums(procstat *proclist, int numprocs,
     unsigned long* memsum)
{
    *memsum = 0;
    for ( int i = 0; i < numprocs; i++ ) {
        *memsum += proclist[i].rss;
    }
}



void show_summary_line1(WINDOW *win)
{
    char buf[128];
    int  nusers;

    /* Get and print current time. */
    mvwaddstr(win, 0, 0, "top - ");
    get_curtime(buf);
    if ( NULL == buf )
        waddstr(win, "          ");
    else
        waddstr(win, buf);
    /* Get and print uptime (from /proc/uptime). */
    get_uptime(buf);
    waddstr(win, buf);

    /* Get and print number of users from utmpx file. */
    nusers = get_numusers();
    if ( 0 == nusers )
        waddstr(win, "No users, ");
    else if ( 1 == nusers )
        waddstr(win, "1 user, ");
    else
        wprintw(win, "%d users, ", nusers);
    /* Get and print load averages (from /proc/loadavg). */
    get_loadavges(buf);
    waddstr(win, buf);
    wrefresh(win);
}

/* Show number of tasks and which are running, sleeping, stopped or
     zombies. */
void show_summary_line2(WINDOW *win, procstat* proctab, int numprocs)
{
    int count[4] = {0,0,0,0};
    enum states{RUNNING, SLEEPING, STOPPED, ZOMBIE};
    mvwaddstr(win, 1, 0, "Tasks: ");
    wprintw(win, "%d total,", numprocs);

    for ( int i = 0; i < numprocs; i++ ) {
        switch ( proctab[i].state  ) {
            case 'R' : count[RUNNING]++; break;
            case 'S' :
            case 'I' :
            case 'D' : count[SLEEPING]++; break;
            case 'T' : count[STOPPED]++; break;
            case 'Z' : count[ZOMBIE]++; break;
        }
    }
    wprintw(win, " %d running, %d sleeping, %d stopped, %d zombie ",
          count[RUNNING], count[SLEEPING], count[STOPPED], count[ZOMBIE]);
    wrefresh(win);
}

void show_summary_line3(WINDOW *win)
{
    int cpustate[8];
    static int prev_cpustate[8] = {0,0,0,0,0,0,0,0};
    double df[8];
    int hz = get_hertz();
    double sum = 0;
    int i;

    mvwaddstr(win, 2, 0, "%Cpu(s): ");
    if ( 8 != get_cpustates( cpustate ) )
        waddstr(win, "Could not get cpu state information");
    else {
        for (  i = 0; i < 8; i++) {
            df[i] = (cpustate[i] - prev_cpustate[i]) / (1.0* hz);
            sum += df[i];
        }
        for (  i = 0; i < 8; i++)
            df[i] = 100 * df[i]/sum;

        for (  i = 0; i < 8; i++) {
            prev_cpustate[i] = cpustate[i];
        }
        wprintw(win, " %2.1f us, %2.1f sy, %2.1f ni, %2.1f id,"
                     " %2.1f wa, %2.1f hi, %2.1f si, %2.1f st,",
                     df[0], df[2], df[1], df[3], df[4], df[5], df[6], df[7]);
        }
    wrefresh(win);
}

void show_summary_line4_5(WINDOW *win)
{
    char line4[128];
    char line5[128];
    get_mem_summary(line4, line5);
    mvwaddstr(win, 3, 0, line4);
    mvwaddstr(win, 4, 0, line5);
    wrefresh(win);

}

void show_summary(WINDOW *win, procstat* proctab, int numprocs)
{
    show_summary_line1(win);
    show_summary_line2(win, proctab, numprocs);
    show_summary_line3(win);
    show_summary_line4_5(win);
    if ( ! show_err ) {
        wmove(win, 5, 0);
        wclrtoeol(win);
    }
    show_err = FALSE;
    wrefresh(win);
}



int isprocdir(  const struct dirent *direntp)
{
    struct stat    statbuf;
    char*   accepts="0123456789"; /* For matching directory names           */


#ifdef _DIRENT_HAVE_D_TYPE
    if( direntp->d_type  == DT_DIR)
#else
    stat(direntp->d_name, &statbuf);
    if ( S_ISDIR(statbuf.st_mode)  )
#endif
        if ( strspn(direntp->d_name, accepts) == strlen(direntp->d_name))
            return 1;
        else
            return 0;
    else
        return 0;
}

void loadprocs(procstat** proclist,  int *numprocs)
{
    struct dirent **namelist;     /* Array of names of proc directories    */
    char    pathname[PATH_MAX];   /* Pathname to file to open               */
    FILE*   fp;                   /* File stream to read                    */
    char*   buf;
    struct  stat  statbuffer;
    size_t  len = MAX_LINE;       /* Length of line getline() returned      */
    unsigned long memtotal = 0;
    int i,j;
    savedcpu *prevcpu_times = NULL;
    int    prevnumprocs = 0;
    long   *diff;
    int    numdirs;

    errno = 0;
    if ( (numdirs = scandir("/proc", &namelist, isprocdir, NULL) ) < 0) {
        cleanup_exit(errno, "scandir");
    }

    /* If a proclist was created already, copy all cpu times into savedcpu
       and free its memory. */
    if ( (*proclist) != NULL ) {  /* loadprocs called before */
        if ( prevcpu_times != NULL )  /* Not first time loadprocs is called */
            free(prevcpu_times);      /* Free previously alloc-ed list */

        prevcpu_times = (savedcpu*) calloc(*numprocs, sizeof(savedcpu));
        if ( NULL == prevcpu_times) {
            cleanup_exit(errno, "calloc");
        }
        /* Copy utime + stime into savedcpu for each process. */
        for ( i = 0; i < *numprocs; i++ ) {
             prevcpu_times[i].pid = (*proclist)[i].pid;
             prevcpu_times[i].cputime = (*proclist)[i].utime +
                                          (*proclist)[i].stime;
        }
        prevnumprocs = *numprocs;
        free(*proclist);
    }


    if ( ( *proclist = (procstat*) calloc(numdirs, sizeof(procstat))) == NULL )
        cleanup_exit(errno, "calloc");

    if ( ( diff = (long*) calloc(numdirs, sizeof(long))) == NULL )
        cleanup_exit(errno, "calloc");

    if ( NULL == (buf = calloc(MAX_LINE, 1))) /* Allocate buffer for getline() */
        cleanup_exit(errno, "malloc");
    j = 0;
    for ( i = 0; i < numdirs; i++ ) {
        memset(pathname, '\0', PATH_MAX);
        sprintf(pathname, "/proc/%s/",namelist[i]->d_name);
        if ( -1 == stat(pathname, &statbuffer)) {
            free(namelist[i]);
            continue;
        }
        (*proclist)[j].uid = statbuffer.st_uid;

        sprintf(pathname+strlen(pathname), "stat");
        if ( NULL == (fp = fopen(pathname, "r") )) {
            /* It's possible that the process ended between the time
                scandir() ran and this attempt was made. Skip this process. */
            free(namelist[i]);  /* Remove this directory now. */
            continue;
        }
        if ( -1 == getline(&buf,&len, fp ) ) {
            cleanup_exit(errno, "getline()");
        }
        if ( 0 == parse_buf(buf, &((*proclist)[j])) ) {
            free(namelist[i]);
            continue;
        }

        if ( 0 == get_procmem_usage((*proclist)[j].pid, &((*proclist)[j].vsize),
             &((*proclist)[j].rss), &((*proclist)[j].shared) ) ) {
            free(namelist[i]);
            continue;
         }
        memtotal += (*proclist)[j].rss;
        memset(buf, 0, MAX_LINE);
        free(namelist[i]);
        fclose(fp);
        j++;
    }
    *numprocs = j;

    if ( prevnumprocs > 0 ) {
        for ( i = 0; i < *numprocs; i++) {
            for ( j = 0; j < prevnumprocs; j++ ) {
                 if ( prevcpu_times[j].pid == (*proclist)[i].pid )
                     /* compute difference in cpu time and save */
                     diff[i] = ((*proclist)[i].utime + (*proclist)[i].stime) -
                        prevcpu_times[j].cputime;
            }
        }
    }
    else {  /* first time -- handle differently */
        for ( i = 0; i < *numprocs; i++)
            diff[i] = ((*proclist)[i].utime + (*proclist)[i].stime);
    }
    for ( i = 0; i < *numprocs; i++) {
        (*proclist)[i].cpu_pct = 100.0 * diff[i] / (delaysecs*ticks);
    }

    for ( i = 0; i < *numprocs; i++) {
        (*proclist)[i].mem_pct = 100.0* ((double) (*proclist)[i].rss) / memtotal;
    }
    free(diff);
    if (prevcpu_times != NULL) free(prevcpu_times);
    free(namelist);
    free(buf);
}

void print_procs( WINDOW* win, procstat* proclist,  int numprocs, int win_lines,
                 int start, uid_t filter, fieldmask fmask)
{
    char    psline[MAX_LINE];
    int i = start ;

    int   numprintlines = MIN(win_lines, numprocs);

    //if ( filter == -1 ) {
        //while ( i < numprintlines + start) {
            //memset(psline, 0, MAX_LINE);
            //print_one_proc(fieldtab, proclist[i], fmask, psline);
            //mvwaddnstr(win, i-start, 0, psline, COLS);
            //i++;
        //}
    //}
    //else {
        int count = 0;
        while ( (i < numprocs) && (count < numprintlines + start) ){
            if ( ( filter == -1) || (proclist[i].uid == filter) ) {
                memset(psline, 0, MAX_LINE);
                print_one_proc(fieldtab, proclist[i], fmask, psline);
                mvwaddnstr(win, count-start, 0, psline, COLS);
                count++;
            }
            i++;
        }
    //}
}

int iowait (struct timespec *ts) {
    fd_set fds;                  /* A descriptor set for pselect     */
    int rc;                      /* Return code from pselect()       */
    sigset_t empty_mask;         /* Signal mask to pass to pselect() */
    char  mssge[32];             /* A message to be output on exit   */

    FD_ZERO(&fds);                /* fs contains standard input only. */
    FD_SET(STDIN_FILENO, &fds);
    sigemptyset(&empty_mask);    /* Make empty mask.                 */

    /* Block until either time expires, inpu available, or signal
       delivered. */
    rc = pselect(1, &fds, NULL, NULL, ts, &empty_mask);
    if ( rc < 0 )  { /* Error from pselect() */
        if ( errno != EINTR )  /* Not an interrupt. Clean up and exit. */
            cleanup_exit(errno, "pselect");
        else  /* An interrupt to pselect(). */
            if ( caught_signal) {  /* Handler ran and set the flag. */
                sprintf(mssge, "Caught signal %d", sigcaught);
                cleanup_exit(-1, mssge); /* Clean up and exit. */
            }
            else
                rc = 0; /* Send 0 instead of -1 to caller. */
    }
    return rc;
}


int pick_user( WINDOW* win, char* user )
{
    int  uid;
    do {
        mvwaddstr(win, 5,0, "Enter a username (blank for all): ");
        echo();
        wrefresh(win);
        wgetnstr(win, user, 31);
        if ( strlen(user) == 0 ) {
            uid = -1;
            break;
        }
        else
            if ( (uid = name2uid(user)) < 0 ) {
                wmove(win, 5, 0);
                wclrtoeol(win);
                mvwaddstr(win, 5,0, "Invalid user");
                //wmove(win, 5, 0);
                mvwchgat(win, 5, 0, strlen("Invalid user"), A_STANDOUT, 0, NULL);
                show_err = TRUE;
                uid = -1;
                break;
            }
            else {
                wmove(win, 5, 0);
                wclrtoeol(win);
                break;
            }
    } while ( TRUE );
    wrefresh(win);
    noecho();
    return uid;
}

void cleanup( int signum )
{
    caught_signal = 1;
    sigcaught = signum;
}

void setup_sighandlers()
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = cleanup;
    if ( ( sigaction(SIGINT, &sa, NULL) == -1 ) ||
       ( sigaction(SIGQUIT,  &sa, NULL) == -1 ) ||
       ( sigaction(SIGSEGV,  &sa, NULL) == -1 ) ||
       ( sigaction(SIGHUP,   &sa, NULL) == -1 ) ||
       ( sigaction(SIGWINCH, &sa, NULL) ) )
         fatal_error(errno, "sigaction");
}

void create_sigmask(sigset_t *sigmask)
{
    sigemptyset(sigmask);
    sigaddset(sigmask, SIGHUP);
    sigaddset(sigmask, SIGINT);
    sigaddset(sigmask, SIGQUIT);
    sigaddset(sigmask, SIGWINCH);
    /* Can add others here. */
}

void setup_curses()
{
    initscr();   /* Initialize curses.    */
    cbreak();    /* Put into cbreak mode. */
    noecho();    /* Turn off echo.        */
    curs_set(0); /* Hide cursor.          */
}

void create_windows(WINDOW **sum_win, WINDOW **head_win, WINDOW **cnt_win )
{
    /* Create a window containing summary in the top  rows of the screen.  */
    if ( NULL == (*sum_win   = newwin(SUMMARY_HEIGHT, COLS, 0, 0)) )
        cleanup_exit(-1, "Could not create summary window.");

    /* Create a  window for the heading.        */
    if ( NULL == (*head_win   = newwin(1, COLS, SUMMARY_HEIGHT, 0)) )
        cleanup_exit(-1, "Could not create heading window.");

    /* Create a content window in the remaining rows of the screen.     */
    if ( NULL == (*cnt_win = newwin(LINES-SUMMARY_HEIGHT-1, COLS,
        SUMMARY_HEIGHT+1, 0)) ) {
        cleanup_exit(-1, "Could not create first window.");
    }
}

void configure_windows(WINDOW *sum_win, WINDOW *head_win, WINDOW *cnt_win )
{
    scrollok(sum_win, FALSE);  /* Disable curses scrolling option.      */
    wstandout(head_win);       /* Put heading window into standout mode.*/
    scrollok(cnt_win, FALSE);  /* Disable curses scrolling option.      */
    keypad(cnt_win, TRUE);     /* Enable arrow and function keys.       */
}

int main(int argc, char *argv[])
{
    WINDOW *content_win;         /* The content area                     */
    WINDOW *heading_win;         /* The one-line heading                 */
    WINDOW *summary_win;         /* The summary at the top of the screen */
    char   heading[MAX_LINE];    /* The string storing the heading       */
    struct timespec  delay;      /* Time interval for refreshes          */
    bool   done = FALSE;         /* Controls main loop.                  */
    procstat  *procarray = NULL; /* Array storing process data           */
    int   numprocs = 0;          /* Number of processes in the array     */
    int   contentlines;          /* Number of lines in content window    */
    int   startline = 0;         /* Vertical offset in array of procs    */
    BOOL  sortdir = FALSE;       /* Sort direction                       */
    char  *username;             /* Entered username for filtering       */
    int filter_uid = -1;         /* Userid by which to filter            */
    fieldmask printfields = F_ALL; /* Mask of columns to print           */
    enum field_t sortfield = CPU; /* Sort field, defaulting to CPU %     */
    sigset_t  sigmask;           /* Signals to block during main loop    */

    setup_sighandlers();
    create_sigmask(&sigmask);
    sigprocmask(SIG_BLOCK, &sigmask, NULL );

    setup_curses();
    create_windows(&summary_win, &heading_win, &content_win );
    configure_windows(summary_win, heading_win, content_win );
    printtopheadings(fieldtab, printfields, heading);
    mvwaddstr(heading_win, 0,0, heading);
    wrefresh(heading_win);
    loadprocs(&procarray, &numprocs);
    ticks = get_hertz();

    /* Set default delay to 3 seconds. */
    delaysecs = 3;
    delay.tv_sec = delaysecs;
    delay.tv_nsec = 0;

    if ( NULL == (username = (char*) calloc(32,1) ) )
        cleanup_exit(errno, "calloc");
    username[0] = '\0';

    while ( !done ) {
        show_summary(summary_win, procarray, numprocs);
        wclear(content_win);
        loadprocs(&procarray, &numprocs);
        sortprocs(procarray, numprocs, fieldtab[sortfield].sortfunc , sortdir);
        contentlines = getmaxy(content_win);
        print_procs(content_win, procarray, numprocs, contentlines, startline,
                    filter_uid, printfields);
        wrefresh(content_win);
        if ( iowait(&delay) > 0 )
            switch ( wgetch(content_win) ) {
                case 'q':  done = TRUE; break;
                case 'm':  sortfield = MEM; sortdir = FALSE; startline = 0; break;
                case 'c':  sortfield = CPU; sortdir = FALSE; startline = 0; break;
                case 't':  sortfield = TIME; sortdir = FALSE; startline = 0;break;
                case 'p':  sortfield = PID; sortdir = FALSE; startline = 0; break;
                case 'u':  sortfield = USER; sortdir = FALSE; startline = 0; break;
                case 'r':  sortdir   = ~sortdir; startline = 0;  break;
                case 'U':
                    filter_uid = pick_user(summary_win, username);
                    startline = 0;
                    break;
                case KEY_DOWN:
                case 'F':
                     if ( startline < numprocs - contentlines)
                         startline++;
                     break;
                case KEY_UP:
                case 'B':
                     if ( startline > 0 )
                         startline--;
                     break;
        }
        wrefresh(heading_win);
    }
    endwin();
    return 0;
}
