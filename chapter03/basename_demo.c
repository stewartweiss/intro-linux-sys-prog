#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[])
{
    char *progname;
    
    progname = basename(argv[0]);
    printf("Program name is %s\n", progname);
    return 0;
}
