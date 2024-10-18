
#include "common_hdrs.h"

int main(int argc, char *argv[])
{
    int fd;
    char buf[2];

    if ( argc < 2 )
        printf("usage: %s file\n", argv[0]);
    else {
        errno = 0;
        fd = open(argv[1], O_RDONLY);
        if ( -1 == fd )
            fatal_error(errno, "open");
        errno = 0;
        if ( -1 == read(fd, buf, 1) )
            perror("read() was not successful");
        else
            printf("read() was successful.\n");

        errno = 0;
        if ( -1 == close(fd) )
            fatal_error(errno, "close");;
    }
    return 0;
}
