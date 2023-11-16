/******************************************************************************
  Title          : getopt_demo.c
  Author         : Stewart Weiss
  Created on     : November 2, 2006
  Description    : Displays messages to prove that command options were parsed
  Purpose        : To demonstrate how to use the getopt() library
                   function to parse command line options within a
                   C or C++ program
  Usage          : getopt_demo
                   with any options following it, but the ones recognized are:
                        -a
                        -b [optional_arg]
                        -c required_arg
                        -d required_arg
                        -1
                    and any number of arguments after them

  Notes
        The getopt() function parses the command-line arguments.  Its arguments
        argc  and argv are the argument count and array as passed to the main()
        function on program invocation.  An element of argv  that  starts  with
        '-' (and is not exactly "-" or "--") is an option element.  The charac‐
        ters of this element (aside from the initial '-')  are  option  charac‐
        ters.   If  getopt() is called repeatedly, it returns successively each
        of the option characters from each of the option elements.

        The variable optind is the index of the next element to be processed in
        argv.  The system initializes this value to 1.

        When there are no more option characters, getopt() returns -1. Then
        optind is the index in argv of the first argv-element that is not an
        option. ( By default, getopt() permutes the contents of argv as it scans,
        so that eventually all the nonoptions are at the end.)

        Copyright (C)  2006  Stewart Weiss

        This program is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <https://www.gnu.org/licenses/>.



******************************************************************************/
#define _GNU_SOURCE
#include <stdio.h>     /* for printf            */
#include <stdlib.h>    /* for exit              */
#include <unistd.h>    /* required for getopt() */

#define  TRUE  1
#define  FALSE 0


int main( int argc, char* argv[])
{
    int ch;
    char options[] = ":hb::c:d:1";
    /* The string is interpreted as follows:
       A letter by itself is an option without arguments, e.g. a is -a

       A letter with a single colon ':' after it has a REQUIRED argument and
       getopt() will place a pointer to the argument in optarg  or if it is
       missing, will return '?' (see below about the leading ':').

       A letter with a double colon after it '::' has an OPTIONAL argument
       and getopt() will place a pointer to it in optarg or will set optarg
       to 0 if it is missing.

       If getopt() finds an undefined option, it will put the character in the
       variable optopt, print a message on stderr, and return '?'. You can
       suppress the message by setting opterr to 0 (as is done below.) It will
       also do this if a required option argument is missing.

       If the leading character is a colon':', then if getopt() finds a missing
       required option argument, instead of returning a '?', it returns a ':',
       which makes it possible to distinguish the type of error.
    */

    opterr = 0;  /* turn off error messages by getopt() */

    while  (TRUE) {
        /* call getopt, passing argc and argv and the options string */
        ch = getopt(argc, argv, options);

        /* it returns -1 when it finds no more options  */
        if ( -1 == ch )
            break;

        switch ( ch ) {
        case 'h':                        /* is a switch (no arg) */
            printf("Found option -h\n");
            break;
        case 'b':                         /* has optional argument */
            if ( 0 != optarg )
                printf("Found option -b with %s\n", optarg);
            else
                printf("Found option -b with no optional argument.\n");
            break;
        case 'c':                        /* has required argument */
            printf("Found option -c with argument %s\n", optarg);
            break;
        case 'd':                        /* has required argument */
            printf("Found option -d with argument %s\n", optarg);
            break;
        case '1':                        /* is a switch (no arg) */
            printf("Found option -1\n");
            break;
        case '?' :
            printf("Found invalid option %c\n", optopt);
            break;
        case ':' :
            printf("Missing required argument\n");
            break;
        default:
           printf ("?? getopt returned character code 0%o ??\n", ch);
           break;
        }
    }

    /* There are no more options: time to check if there are arguments */
    /* on the rest of the command line. The integer variable optind  */
    /* is the index of the first non-option word in the argv[] array */
    /* If optind < argc, there is at least one word that is not an option */

    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        printf ("\n");
    }

    return 0;
}

