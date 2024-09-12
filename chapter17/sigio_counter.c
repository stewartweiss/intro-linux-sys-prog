/*****************************************************************************
  Title          : sigio_counter.c
  Author         : Stewart Weiss
  Created on     : August 3,2024
  Description    : Displays a count of SIGIO signals and shows characters read
  Purpose        : To show what "input possible" means on a terminal's fd
  Usage          : sigio_counter
  Build with     : gcc -Wall -L../lib -I../include -o sigio_counter \
                     sigio_counter.c -lspl
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
#include <fcntl.h>

volatile sig_atomic_t input_ready = 0;
volatile int   count = 0;

void setup_fd(int fd )
{
    int flagset   = fcntl(fd, F_GETFL);
    if ( flagset == -1 )
        fatal_error(errno, "fcntl");
    if ( -1 == fcntl(fd, F_SETFL, flagset | O_ASYNC | O_NONBLOCK))
        fatal_error(errno, "fcntl");
    fcntl(fd, F_SETOWN, getpid());
}

void on_input(int signum)
{
    input_ready = 1;
    count++;
}

int main( int argc, char * argv[])
{
    struct sigaction sigact;
    sigset_t   blockedsigs;
    char   ch;
    BOOL   finished = FALSE;

    sigemptyset(&blockedsigs);                  // Create the signal mask
    sigaddset(&blockedsigs, SIGIO);             // containing SIGIO.
    sigact.sa_handler = on_input;               // Establish the SIGIO
    sigact.sa_flags   = SA_RESTART;             // handler.
    sigemptyset(&sigact.sa_mask);
    if ( sigaction(SIGIO, &sigact, NULL) == -1 )
        fatal_error(errno, "sigaction");
    setup_fd(STDIN_FILENO);                     /* Set up signal-driven I/O */
    while( !finished  ) {
        pause();
        if ( input_ready ) {                    /* SIGIO delivered          */
            input_ready = 0;
            sigprocmask(SIG_BLOCK, &blockedsigs, NULL); /* Block it.        */
            while ( read(STDIN_FILENO, &ch, 1)  > 0  && !finished ) {
                if ( ch == 'q' )
                    finished = TRUE;
                printf("SIGIO count = %d; current char = %c\n", count, ch);
            }
            sigprocmask(SIG_UNBLOCK, &blockedsigs, NULL); /* Unblock it.    */
        }
    }
    return 0;
}
