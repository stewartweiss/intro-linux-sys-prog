/******************************************************************************
  Title          : forkdemo3.c
  Author         : Stewart Weiss
  Created on     : Oct. 5, 2001
  Description    : Simple example of fork() system call
  Purpose        : To demonstrate that fork() creates a new process. How many
                   processes will run when the loop upper bound is N? Try
                   answering this before running the program.
  Usage          : forkdemo3  [N]
                   where N is a positive integer, not too large or you will
                   run out of resources in your shell. (Keep it under 10)

  Build with     : gcc -Wall -g -o forkdemo3 forkdemo3.c
  Modifications  : May 4, 2012
                   Made the number of loop iterations a command-line argument
                   Changed the output instructions to prevent race conditions.



******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include   <fcntl.h>
#include <termios.h>


int main( int argc, char* argv[] )
{
    int    i;
    int    N;
    char   str[32];
    int    flags;

    /* Put standard output into atomic append mode */
    flags = fcntl(1, F_GETFL);
    flags |= (O_APPEND);
    if (fcntl( STDOUT_FILENO, F_SETFL,flags) == -1 )
        exit(1);

    /* Get the command line value and convert to an int.
       If none, use default of 4. If invalid, exit. */
    N = ( argc > 1 )? atoi(argv[1]):4;
    if ( 0 == N )
        exit(1);

    /* Print a message and flush it if this is not a terminal */
    printf("About to create many processes...\n");
    if ( !isatty(fileno(stdout)) )
        fflush(stdout);

    /* Now fork the child processes. Check return values and exit
       if we have a problem. Note that the exit() may be executed
       only for some children and not others. */
    for ( i = 0; i < N; i++ )
        if ( -1 == fork() )
            exit(1);

    /* Create the output string that the process wil/ write, and write using
       system call. */
    sprintf(str, "Process id = %d\n", getpid());
    write(1, str, strlen(str));
    fflush(stdout);        /* to force output before shell prompt */
    sleep(1);              /* to give time to the shell to diplay prompt */
    return 0;
}
