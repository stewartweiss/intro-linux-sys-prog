#include <stdio.h>
#include <string.h>

void main(int argc, char * argv[])
{
    char *forwardslashptr;
    char *suffixptr = NULL;

    forwardslashptr = strrchr( argv[0], '/');
    if ( forwardslashptr != NULL ) 
        suffixptr = forwardslashptr+1;
    else
        suffixptr = argv[0];

    if ( suffixptr == NULL )
        fprintf(stderr, "Program name ends in a / character\n");
    else
        printf("Program name is %s\n", suffixptr);
}
