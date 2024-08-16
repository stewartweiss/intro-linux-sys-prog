/******************************************************************************
  Title          : nonblockdemo.c
  Author         : Stewart Weiss
  Created on     : April 11, 2013
  Description    : Displays a pattern on screen and also reads user input
  Purpose        : Demonstrates use of the O_NONBLOCK flag for nonblocking input
  Usage          : nonblockdemo
  Build with     : gcc -o nonblockdemo nonblockdemo.c
  Modifications  :

  Notes          :
  This just draws a sequence of dots on the screen while displaying a prompt
  asking the user to enter a command. The user can type:
  q to quit
  p to pause,
  r to resume,
  any punctuation character, to change what' printed to that character.
  The whole point is just to show how non-blocking mode on the input file
  descriptor can be used while the program is busy doing something else.



******************************************************************************/

#include "common_hdrs.h"
#include <fcntl.h>


void set_non_block(int fd )
{
    int flagset;

    flagset   = fcntl(fd, F_GETFL);
    if ( flagset == -1 )
        fatal_error(errno, "fcntl");
    flagset   |= O_NONBLOCK;
    if ( -1 == fcntl(fd, F_SETFL, flagset))
        fatal_error(errno, "fcntl");
}

void moveto(int line, int col )
{
    char seq_str[20];
    sprintf(seq_str, "\033[%d;%dH", line, col);
    write(STDOUT_FILENO, seq_str, strlen(seq_str));
}

int main (int argc, char *argv[])
{
    char   ch;
    int    count = 0;
    int    done  = 0;                /* To control when to stop loop */
    char   str[128];
    const  char CLEAR_SCREEN[] = "\033[2J";
    const  char CLEAR_LINE[]   =  "\033[1A\033[2K\033[G";
    set_non_block( STDIN_FILENO );   /* Turn off blocking mode. */
    write(STDOUT_FILENO,CLEAR_SCREEN, strlen(CLEAR_SCREEN));
    moveto(1,1);
    while ( !done  ) {
        count++;
        usleep(500000);  /* Delay a bit. */
        /* Do the read. If nothing was typed, print a message. */
        write(STDOUT_FILENO,CLEAR_LINE, strlen(CLEAR_LINE));
        if ( read(STDIN_FILENO, &ch, 1) > 0 ) {
            if ( ch == 'q' )
                done = 1;
            else if ( ch != '\n') {
                moveto(2,1);
                sprintf(str, "\rUser entered %c\n", ch);
                write(1, str, strlen(str));
                moveto(1,1);
            }
        }
        else {
            moveto(3,1);
            sprintf(str, "\rNo input; number of unsatisfied reads = %d\n",
                    count);
            write(1, str, strlen(str));
            moveto(1,1);

        }
    }
    write(STDOUT_FILENO,CLEAR_SCREEN, strlen(CLEAR_SCREEN));
    return 0;
}





