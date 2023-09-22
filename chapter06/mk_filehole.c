#define _GNU_SOURCE
#include "../include/common_hdrs.h"
#include "../include/common_defs.h"

#define FILENAME       "file_with_hole"
#define MESSAGE_SIZE   512
#define BUFFER_SIZE     10

int main(int argc, char *argv[])
{
    int     fd;
    char    buffer[BUFFER_SIZE];
    char    message[MESSAGE_SIZE];

    /* Create a new file named file_with_hole in the pwd. */
    if ((fd = open(FILENAME, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0) {
        sprintf(message, "unable to open file %s for writing", FILENAME);
        fatal_error(errno, message);
    }

    /* Fill buffer with a small string. */
    strncpy(buffer, "0123456789",BUFFER_SIZE);

    /* Write the small string at the beginning of the file. */
    if (write(fd, buffer, BUFFER_SIZE) != BUFFER_SIZE)
        fatal_error(errno, "write");

    /* Seek 1,000,000 bytes past the end of the file. */
    if (lseek(fd, 1000000, SEEK_END) == -1)
        fatal_error(errno, "lseek");

    /* Write the small string at the new file offset. */
    if (write(fd, buffer, BUFFER_SIZE) != BUFFER_SIZE)
        fatal_error(errno, "write");

    /* Close the file. */
    if ( close(fd) == -1 ) {
        sprintf(message, "error closing file %s\n", FILENAME);
        fatal_error(errno, message);
      }

    /* We now have a large file with a big hole. */
    exit(EXIT_SUCCESS);
}
