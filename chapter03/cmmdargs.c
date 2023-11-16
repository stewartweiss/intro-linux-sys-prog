/*
 * cmmdargs.c -- shows how to access command line arguments and
 *               environment variable strings
 * Author:       Stewart Weiss
 * Date Created: April 4, 2003
 * Revisions:    Revised January 24, 2006
 *               -- formatted for clarity 
 *               -- self-documenting output
 */
 
#include <stdio.h>

int main(int argc, char * argv[], char * envp[])
// The system call that starts all processes passes these 3 arguments 
// to the process. The first is the size of the second argument as an
// integer, and the second is an array of character strings representing
// the words on the command line, and the third is an array of character
// strings of the form "variable=value" from the process's environment.

{ 
    int n = 0;
    int i = 0;
 
    printf("\n");
    while ( argv[i++] != NULL )
         printf("Command line word #%d is %s\n", i, argv[i]);

    printf("\n");
    printf("The environment variables passed to this process are:\n\n");
    while (envp[n] != 0 ) {
        printf("%s\n",envp[n++]);
    }
    printf("\n");
    return 0;
  
}
