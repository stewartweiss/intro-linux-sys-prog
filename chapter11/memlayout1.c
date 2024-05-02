#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



extern char etext, edata, end;

int main(int argc, char *argv[])
{
    char command[32];
    sprintf(command, "cat /proc/self/maps");
    system(command);
    return 0;
}



