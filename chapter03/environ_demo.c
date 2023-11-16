#include <stdlib.h>
#include <stdio.h>

/* Declare the environ variable to be able to access it 
   It must be declared extern because it is defined outside
   of the program
*/
extern char **environ;

int main()
{
    char **envp = environ; /* set point to start of list */
    while ( NULL != *envp ) {
        printf("%s\n", *envp );
        envp++;
    }
    return 0;
}
