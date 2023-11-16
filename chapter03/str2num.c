#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char* argv[])
{
    int x,y;
    
    if ( argc < 3 ) {
        exit(1);
    }
    sscanf(argv[1], " %d", &x);
    sscanf(argv[2], " %d", &y);
    printf("The numbers are %d and %d\n", x, y);
    return 0;
}
