/*****************************************************************************
  Title          : ulogger.c
  Author         : Stewart Weiss
  Created on     : June 16, 2024
  Description    : Displays simulated list of users logged in
  Purpose        : To show how to respond asynchronously to message queue
  Usage          : ulogger
                   Then, in another terminal window, run ulogger_client
                   in the form
                         ulogger_client  username line
  Build with     : gcc -Wall -g -I../include -L../lib  -o ulogger \
                    ulogger.c -lrt
  Modifications  :
     11/28/2025 by SNW: Put parentheses around macro SIGRTMIN+1

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

#include "common_hdrs.h"
#include <pthread.h>
#include <mqueue.h>
#include <sys/wait.h>
#include <termios.h>
#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif
#include "ulogger.h"

#define  MAX_TIMESTR   16
#define  MAX_USERS     256
#define  SIGMSGAVAIL   SIGRTMIN
#define  SIGUPDATE     (SIGRTMIN+1)  /* Added parentheses 11/28/2025 */

typedef struct _user {
    uid_t   uid;
    char    nickname[MAX_NAME];
    char    line[MAX_LINE];
    char    start_time[MAX_TIMESTR];
} uinfo;


char *          msg_buffer;
ssize_t         msg_size;
mqd_t           mqdes;
struct sigevent sev;
uinfo           users[MAX_USERS];
int             count  = 0;
unsigned short int rows, cols;

msgtype         newmsg;

char CLEAR_SCREEN  [] = "\033[2J\033[3J";

void moveto(int line, int col )
{
    char seq_str[20];

    sprintf(seq_str, "\033[%d;%dH", line, col);
    write(STDOUT_FILENO, seq_str, strlen(seq_str));
}

void clearscreen()
{
    moveto(rows,1);
    write(STDOUT_FILENO, CLEAR_SCREEN, strlen(CLEAR_SCREEN));
}

void print_status_line(char * start, char* name, uid_t uid)
{
    char     buf[128];
    clearscreen();
    moveto(rows,1);
    sprintf(buf, "Arrived at %s:  Nickname = %s (UID = %d)",
             start,  name, uid);
    write(STDOUT_FILENO, buf, strlen(buf));
}
/* Create a new entry in the global users[] array for the message msg. */
void update( msgtype *msg, uid_t uid, char *start  )
{
    if ( count == MAX_USERS )
         error_mssge(-1, "Maximum number of allowed users reached.");
     else {
        strcpy(users[count].nickname, msg->name);
        users[count].uid = uid;
        strcpy(users[count].start_time, start);
        strcpy(users[count].line, msg->line);
        count++;
     }
}

void msg_handler( int signo, siginfo_t *info, void *context )
{
    ssize_t  nbytes;
    time_t   arrival_time;
    struct   tm *bdtime;
    char     timestr[MAX_TIMESTR];

    if ( info->si_code != SI_MESGQ )
        fatal_error(-1, "Signal handler invoked but not for arriving message");

    /* Get current time and convert to string. */
    time(&arrival_time);
    bdtime = localtime(&arrival_time);
    strftime(timestr, MAX_TIMESTR, "%X", bdtime);

    memset(msg_buffer, 0, msg_size);
    nbytes = mq_receive(mqdes,  msg_buffer, msg_size, NULL);
    if ( nbytes != -1 ) {
        newmsg = *((msgtype*) msg_buffer);
        print_status_line(timestr, newmsg.name, info->si_uid);
        update(&newmsg, info->si_uid, timestr);
    }
    if (mq_notify(mqdes, &sev) == -1)
       fatal_error(errno, "mq_notify");

    while ( -1 != mq_receive(mqdes, msg_buffer, msg_size, NULL) )
        continue;
    if ( errno != EAGAIN )
       fatal_error(errno, "mq_receive");
    if ( nbytes != -1 )
        raise(SIGUPDATE);  /* Force display of message content. */
}

void cleanup(int signo )
{
    free(msg_buffer);
    if ( -1 == mq_unlink(mqname) )
        fatal_error(errno, "mq_unlink");
    clearscreen();
    exit(EXIT_SUCCESS);
}

void setup_sighandlers(struct sigaction *sa, int flags)
{
    sigemptyset(&sa->sa_mask);
    sa->sa_handler = cleanup;
    sa->sa_flags = flags;
    if ( ((sigaction(SIGINT,  sa, NULL)) == -1 ) ||
         ((sigaction(SIGHUP,  sa, NULL)) == -1 ) ||
         ((sigaction(SIGQUIT, sa, NULL)) == -1)  ||
         ((sigaction(SIGTERM, sa, NULL)) == -1)     )
         fatal_error(errno,"sigaction");
}

void get_winsize(int fd, unsigned short *rows, unsigned short *cols )
{
    struct winsize size;

    if (ioctl(fd, TIOCGWINSZ,  &size) < 0)
        fatal_error(errno, "TIOCGWINSZ error");
    *rows = size.ws_row;
    *cols = size.ws_col;
}

void print_column_headings()
{
    moveto(1,1);
    printf("%-*s %-*s %-*s\n", MAX_NAME, "Name",
                       MAX_LINE, "Line", MAX_TIMESTR, "Time");
}


int  main(int argc, char *argv[])
{
    struct mq_attr attr;
    struct sigaction sigact;
    sigset_t  mask;
    int     signo;

    /* Only run this without redirection. */
    if (isatty(STDIN_FILENO) == 0)
        fatal_error(-1, "Not a terminal\n");

    get_winsize(STDIN_FILENO, &rows, &cols);

    mqdes = mq_open(mqname, O_CREAT |  O_RDONLY | O_NONBLOCK, 0660, NULL );
    if (mqdes == (mqd_t) -1)
       fatal_error(errno, "mq_open");

    if (mq_getattr(mqdes, &attr) == -1)
       fatal_error(errno, "mq_getattr");
    msg_size = attr.mq_msgsize;

    msg_buffer = malloc(attr.mq_msgsize);
    if (msg_buffer == NULL)
       fatal_error(errno, "malloc");

    sigact.sa_sigaction = msg_handler;
    sigact.sa_flags = SA_SIGINFO;
    sigemptyset(&(sigact.sa_mask));
    if ( sigaction(SIGMSGAVAIL, &sigact, NULL) == -1 )
        fatal_error(errno, "sigaction");

    sigemptyset(&mask);
    sigaddset(&mask, SIGUPDATE);
    if ( -1 == sigprocmask(SIG_BLOCK, &mask, NULL) )
        fatal_error(errno, "sigprocmask");

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo  = SIGMSGAVAIL;
    if (mq_notify(mqdes, &sev) == -1)
       fatal_error(errno, "mq_notify");

    setup_sighandlers(&sigact, 0);
    clearscreen();

    while ( TRUE ) {
        sigwait(&mask, &signo);  /* Wait for SIGUPDATE signal. */
        if ( signo == SIGUPDATE) {/* This is raised in SIGMSGAVAIL handler. */
            print_column_headings();
            int first = count - rows + 2;
            first = first >= 0 ? first : 0;
            for ( int i = first; i < count; i++ ) {
                moveto(i-first+2,1);
                printf("%-*s %-*s %-*s\n", MAX_NAME, users[i].nickname,
                       MAX_LINE, users[i].line, MAX_TIMESTR,
                       users[i].start_time);
            }
        }
    }
}

