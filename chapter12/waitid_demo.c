
/*****************************************************************************
  Title          : waitid_demo.c
  Author         : Stewart Weiss
  Created on     : April 1, 2023
  Description    : Creates a single child process that can be sent signals
  Purpose        : To show how a process can monitor children with waitid()
  Usage          : waitid_demo
  Build with     : gcc -Wall -I../include -L../lib -g -o waitid_demo \
                          waitid_demo.c -lspl

  Notes          :
                   Once the program starts running, you will have one minute
                   to issue send signals, via a second terminal window, to
                   the child process. Use
                       kill -s SIGSTOP <pid>
                   and
                       kill -s SIGCONT <pid>
                   to see  the parent detect when the child has been stopped
                   and restarted. If you let the sleep timer expire, the child
                   will exit normally. Otherwise you can send a signal to kill
                   it, such as SIGINT or SIGQUIT.
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
#include <sys/wait.h>


#define SLEEPTIME  60

int main(int argc, char* argv[])
{
    pid_t      pid;
    siginfo_t  siginfo;

    if ( -1 == (pid = fork())) {
        perror("fork");
        exit(1);
    }
    else if ( 0 == pid ) {
        printf("Child PID: %d\n", getpid());
        sleep(SLEEPTIME); /* Child just sleeps. */
        exit(0);
    }

    /* Parent code */
    else do {
        /* Zero out si_pid in case the sig_info_t struct does not get */
        /* initialized because no children are waitable. */
        siginfo.si_pid = 0;

        /* Wait for changes in the state of the child created above,  */
        /* specifically, stopping, resuming, exiting, and return */
        /*immediately if no child is waitable. */
        if (-1 == waitid(P_PID, pid, &siginfo, 0
                  | WEXITED
                  | WSTOPPED
                  | WCONTINUED
                //  | WNOHANG
            )) {
            perror("waitid");
            exit(EXIT_FAILURE);
        }
        if ( siginfo.si_pid == 0 )
            /* no child is waitable. */
            continue;
        switch ( siginfo.si_code ) {
	        case CLD_EXITED:
                printf("Child exited with status %d\n",
                        siginfo.si_status );
                break;
	        case CLD_KILLED:
          case CLD_DUMPED:
                printf("Child killed by signal %d\n",
                        siginfo.si_status );
                break;
	        case CLD_STOPPED:
                printf("Child stopped by signal %d\n",
                        siginfo.si_status );
                break;
	        case CLD_CONTINUED:
                printf("Child continued\n");
                break;
        }
    } while ( siginfo.si_code != CLD_EXITED &&
                   siginfo.si_code != CLD_KILLED &&
                   siginfo.si_code != CLD_DUMPED );
    return 0;
}
