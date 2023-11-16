#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int  main(int argc, char *argv[])
{
    char *endptr;
    long val;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s str \n", argv[0]);
        exit(EXIT_FAILURE);
    }
    errno = 0;    /* To distinguish success/failure after call */
    val = strtol(argv[1], &endptr, 0);

    /* Check for various possible errors */
    if (errno != 0) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }
    /* errno == 0 */
    if (endptr == argv[1]) {
        /* the first invalid char is the first char of the string */
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }

    if (*endptr != '\0')
      /* there are non-number characters following the number.
         which we can call an error or not, depending. */
       printf("Characters following the number: \"%s\"\n", endptr);

    /* If we got here, strtol() successfully parsed a number */
    printf("strtol() returned %ld\n", val);
    exit(EXIT_SUCCESS);
}
