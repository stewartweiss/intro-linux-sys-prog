#include "common_hdrs.h"

int main(int argc, char* argv[])
{

    int i, fd;

    for ( i = 0; i < 1024; i++) {
        errno = 0;
        fd = open("/home/stewart/scratch/foo", O_RDONLY);
        if ( fd == -1 ) {
            printf( "i = %d", i);
            fatal_error(errno, "open" );
        }
    }
    return 0;
}
