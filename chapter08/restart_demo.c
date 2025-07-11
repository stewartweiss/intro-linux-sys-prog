/*****************************************************************************
  Title       : restart_demo.c
  Author      : Stewart Weiss
  Created on  : 2011, revised 2023
  Description : Waits for user input and a keyboard interrupt
  Purpose     : To demonstrate what happens when system calls are or are not
                restarted when they get interrupted.
  Usage       : restart_demo
  Build with  : gcc -Wall -I../include -o restart_demo restart_demo.c \
                 -L../lib -lspl

  Notes          :
  To use this program:
    First run it and type no keyboard interrupts, but enter at least 4
    characters followed by Enter.  It will output
    "You entered"
    followed by the first 4 characters that you entered.

    Next, enter fewer than 4 characters followed by ENTER without any 
    keyboard interrupt.  It will output 
    "You entered"
    followed by the characters you entered.

    Now, enter any number of characters (other than ENTER) then Ctrl-C. 
    It will output the message
    "Read call was interrupted." followed by the string XXXX.

    Last, enter any number of characters (not including ENTER) then 
    Ctrl-\. It will first output
    "Signal caught; re-enter input."
    display the prompt ">" and wait for you to aany number of characters 
    followed by Enter. It will output the message
    "Read call was interrupted and restarted." followed by the first 4
    of the new characters that you typed.

******************************************************************************
* Copyright (C) 2025 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include "common_hdrs.h"
#include <signal.h>
#include <termios.h>

volatile sig_atomic_t  got_interrupt= 0;
char alert[] = "\nSignal caught; re-enter input.\n> ";
int  alertlen;

void on_interrupt(int signo)
{
    got_interrupt = 1;
    if ( SIGQUIT == signo )
        write(1, alert, alertlen);
}

int main(int argc, char *argv[])
{
    struct sigaction sigact;
    int n;
    char c[5] = "XXXX";

    alertlen = strlen(alert);
    sigact.sa_handler = on_interrupt;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    if (-1 == sigaction(SIGINT, &sigact, NULL))
        fatal_error(errno, "sigaction()");

    sigact.sa_flags = SA_RESTART;
    if (sigaction(SIGQUIT, &sigact, NULL))
        fatal_error(errno, "sigaction()");

    /* Request a read of 4 chars. */
    n = read(0, &c, 4);

    /* If a SIGINT interrupt was received, SA_RESTART was not set
       so the call will not be restarted. */
    if (-1 == n  &&  EINTR == errno ) {
        printf("Read call was interrupted.\n");
        write(1, c, 4);
        printf("\n");
    }
    /* If a SIGQUIT interrupt was received SA_RESTART was on
       so the call will be restarted. */
    else if (got_interrupt) {
        printf("Read call was interrupted and restarted.\n");
        write(1, c, 4);
        printf("\n");
    }
    else {
        if ( n < 4 )
            c[n] = '\0';
        printf("You entered %s\n", c);
    }

    /* Drain the terminal queues in case there are characters still there. */
    tcflush(0,TCIFLUSH);
    return 0;
}
