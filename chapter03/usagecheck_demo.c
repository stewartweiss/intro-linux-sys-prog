#include <stdio.h>
#include <stdlib.h>

int main ( int argc,  char * argv[]  )
{
    if ( argc < 3 ) { /* too few arguments */
        /* handle the incorrect usage here */
        fprintf(stderr, "usage: %s file1 file2\n", argv[0]);
        exit(1);
    }
    printf("About to copy from %s to %s\n", argv[1], argv[2]);
    /* but no copying yet */
    return 0;
}
