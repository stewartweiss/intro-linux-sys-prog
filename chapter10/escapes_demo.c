/*****************************************************************************
  Title          : escapeseq_demo.c
  Author         : Stewart Weiss
  Created on     : January  2, 2023
  Description    : Demonstrates use of ANSI escape sequences
  Purpose        : To show the effects of various ANSI escape sequences
  Usage          : vt100ctrl
  Build with     : gcc -Wall -g  -L../lib  -I../include -o escapeseq_demo \
                   escapeseq_demo.c  -lspl

  Notes          :
  This program lets the user see the effects of various escape sequences and
  how they can be used. The choice of escapes shown here is pretty random.

  Every ANSI escape sequence begins with the character whose code is
  decimal code 27, or octal 033, which we will write as ESC. Many codes
  begin with a two-character sequence consisting of ESC followed by '[',
  although there are also escape sequences that use just the ESC.


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

#define _GNU_SOURCE
#include "common_hdrs.h"
#include <termios.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

/******************************************************************************/
/*                      Selected VT100 Escape Sequences                       */
/******************************************************************************/

#define CLEAR_LINE      "\033[1A\033[2K\033[G"
#define CLEAR_SCREEN    "\033[2J"
#define CLEAR_ABOVE     "\033[1J"
#define CLEAR_DOWN      "\033[0J"
#define INVERSE_START    "\033[7m"
#define INVERSE_END     "\033[27m"
#define PROMPT            "prompt: "
#define CURSOR_HOME     "\033[1;1H"
#define CURSOR_PARK     "\033[24;80H"
#define CURSOR_UP_2     "\033[2A"
#define CURSOR_DOWN_2   "\033[4B"
#define SCROLL_WINDOW   "\033M"


// Display the menu of choices
void display_menu(FILE* fp)
{
    char * menu =
"v  displays prompt in reverse video         p displays prompt in normal video\n"
"h  homes the cursor (at upper left)         b moves cursor to lower right\n"
"c  clears entire screen                     l clears lines containing cursor\n"
"z  clears from cursor to bottom of screen   u clears screen above the cursor\n"
"+  moves cursor up 1 line                   s moves cursor up 20 lines, \n"
"                                              scrolling if necessary\n"
"?  displays this menu                       q quits\n";
    fprintf(fp, "%s", menu);
}

/******************************************************************************/
/*                           Main Program                                     */
/******************************************************************************/

int main (int argc, char *argv[])
{
    int    v;         // stores user's char
    int    i;
    size_t count;     // not using this now
    char*  line;      // to call getline()
    int    done = 0;  // to control when to stop loop
    FILE*  tty;       // to open terminal

    // Try to open the terminal device file for reading
    if ( (tty = fopen("/dev/tty","r")) == NULL ) {
        fprintf(stderr,"Could not open tty to read!\n");
        exit(EXIT_FAILURE);
    }

    // This is how to test whether output has been redirected:
    if ( !isatty(fileno(stdout)) ) {
        fprintf(stderr,"Output has been redirected!\n");
        exit(EXIT_FAILURE);
    }

    // Dynamically allocate the line. If you are new to this, read the man
    // page for calloc; it takes a number of entities and a size in bytes
    // for each entity. If it returns NULL, bad news.
    line = calloc(512, sizeof(char));
    if ( NULL == line  )
        exit(EXIT_FAILURE);

    // Clear the screen.
    printf(  "%s%s", CLEAR_SCREEN,CURSOR_HOME);

    // Give the user the menu first.
    display_menu(stdout);

    // The main loop. Repeatedly test whether user pressed 'q' to quit and if
    // not, use fgetc to read a single character. Because the user must press
    // the enter key to transmit the keypress to the program (we have not yet
    // learned how to use "unbuffered input"), we have to "consume" the newline
    // character as well. That is the sole purpose of the call to getline() that
    // starts the loop body.
    while ( !done && (v = fgetc(tty)) != EOF  ) {
        // need to read a newline character and discard it
        getline(&line, &count, tty);
        switch ( v ) {
        case 'v':
            printf(  "%s%s%s",INVERSE_START,PROMPT,INVERSE_END);
            break;
        case 'p':
            printf(  "%s",PROMPT);
            break;
        case 'h':
            printf(  "%s%s",CLEAR_LINE, CURSOR_HOME);
            break;
        case '+':
            printf(  "%s",CURSOR_UP_2);
            break;
        case '-':
            printf(  "%s",CURSOR_DOWN_2);
            break;
        case 'c':
            printf(  "%s", CLEAR_SCREEN);
            break;
        case 'b':
            printf(  "%s",CURSOR_PARK);
            break;
        case 'l':
            printf("%s", CLEAR_LINE);
            break;
        case 'z':
            printf("%s", CLEAR_DOWN);
            break;
        case 'u':
            printf(  "\033[1J");
            break;
        case '?':
            printf(  "%s%s", CLEAR_SCREEN,CURSOR_HOME);
            display_menu(stdout);
            break;
        case 's':
            for ( i = 1; i <= 20; i++ )
                printf("%s%s", SCROLL_WINDOW, CURSOR_HOME);
            break;
        case 'q':
            done = 1;
            break;
        default:
            printf(  "%c:No such choice. Press ? for help.\n", v);

        }
    }
    printf("%s\033[24;1H",CLEAR_SCREEN);
    free(line);
    return (0);
}





