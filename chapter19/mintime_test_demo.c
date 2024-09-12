/*****************************************************************************
  Title          : mintime_testing_demo.c
  Author         : Stewart Weiss
  Created on     : July 26, 2024
  Description    : Experiment with MIN and TIME settings in noncanonical mode
  Purpose        : To provide a way to test how settings affect read()
  Usage          : mintime_test_demo [-m M ] [ -t T]
                   where M is MIN value and T is TIME value
  Build with     :

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
#include <termios.h>


/* sets the VMIN value to vmin, VTIME to vtime, turns off icanon */
void set_non_canonical(struct termios *ttystate, int vmin, int vtime)
{
    tcgetattr( 0, ttystate);              /*  Read current setting.   */
    ttystate->c_lflag       &= ~ICANON;   /*  No buffering            */
    ttystate->c_cc[VMIN]     =  vmin;     /*  Set MIN to Vmin .       */
    ttystate->c_cc[VTIME]    =  vtime;    /*  Set TIME to vtime.      */
    if ( -1 == tcsetattr( 0 , TCSANOW, ttystate)  )
        fatal_error(errno, "tcsetattr");
}

void do_read( )
{
    int    num_bytes_read;
    char   input[128];

    fflush(stdout);
    printf("Enter some characters or wait to see what happens.\n");
    while ( 1 ) {
        sleep(2);
        num_bytes_read = read(0, input, 6) ;
        if ( num_bytes_read >= 0 ) {
            input[num_bytes_read] = '\0';
            if ( num_bytes_read > 0 ) {
                printf("Return value of read(): %d; chars read: %s\n",
                    num_bytes_read, input );
                if (input[0] == 'q')
                    break;
            }
            else
                printf("Return value of read():  %d;"
                        " no chars read\n",num_bytes_read);
        }
        else
            printf("read() returned -1\n");
    }
}


int main(int argc, char* argv[])
{
    int    min     = 1;  /* Default is one char. */
    int    time    = 0;  /* Default is to force reads to wait for min chars */
    int    ch;
    char   optstring[] = ":hm:t:";

    struct termios  current, original;

    if (! isatty(STDIN_FILENO) )
        usage_error("No input redirection allowed.");
    if ( tcgetattr(0,&current) == -1 )   /* Retrieve termios struct. */
        fatal_error(errno, "tcgettattr");
    original = current;                  /* Save original termios state. */


    while (1) {
        ch = getopt(argc, argv, optstring);
        if ( -1 == ch )
            break;
        switch (ch) {
        case 'h' :
            printf("Usage: %s [-h] | [ -m MIN ] [ -t TIME ]\n",argv[0]);
            printf("       -h : help\n");
            printf("       -m : set VMIN value \n");
            printf("       -t : set VTIME value \n");
            exit(0);
        case 'm' :
            min = strtol(optarg,'\0',0);
            break;
        case 't' :
            time = strtol(optarg, '\0',0);
            break;
        case '?' :
            break;
        case ':' :
            break;
        default:
           fprintf(stderr, " ");
        }
    }

    set_non_canonical( &current, min, time);
    printf("MIN set to %d, ",min);
    printf("TIME set to %d\n",time);

    do_read();   /* Call function to get input. */

    if ( -1 == tcsetattr( 0 , TCSANOW, &original) ) /* Restore settings. */
        fatal_error(errno, "tcsetattr");
    return 0;
}






