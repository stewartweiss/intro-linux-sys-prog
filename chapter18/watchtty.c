/******************************************************************************
  Title          : ttywatch.c
  Author         : Stewart Weiss
  Created on     : March 19, 2013
  Description    : Keeps a small log of the size of specified terminal queue
  Purpose        : Demonstrates use of ioctl TIOCINQ for getting size of
                   terminal input queue.
  Usage          : ioctl3 terminal-device
  Build with     : gcc -Wall -g -o ioctl3 ioctl3.c
  Modifications  :
  Notes          :
  This is designed to be used in conjunction with ioctl2.c, which removes
  characters from a terminal queue sufficiently sjowly so that this can
  report the distinct sizes.

******************************************************************************/
#include "common_hdrs.h"
#include   <sys/ioctl.h>

int main(int argc, char *argv[] )
{
    int     count;
    int     fd;

    /* Try to open given terminal device file */
    if ((fd = open(argv[1], O_RDONLY)) == -1)
        usage_error("ttywatch  <device-file>");

    while ( TRUE ) {
        if ( ioctl( fd, TIOCINQ, &count ) == -1 )
            fatal_error( errno, "ioctl TIOCINQ");
        printf("%d chars in queue\n", count);
        usleep(100000);  /* Delay to see changes */
    }
    return 0;
}
