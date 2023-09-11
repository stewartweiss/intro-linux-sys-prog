#include <stdio.h>
#include <limits.h>
//#include <dirent.h>

int main()
{

#ifdef MAXNAMLEN
    printf("MAXNAMLEN = %d\n", MAXNAMLEN);
#else
    printf("no MAXNAMLEN\n");
#endif

#ifdef NAME_MAX
    printf("NAME_MAX = %d\n", NAME_MAX);
#else
    printf("no NAME_MAX\n");
#endif

#ifdef PATH_MAX
    printf("PATH_MAX = %d\n", PATH_MAX);
#else
    printf("no PATH_MAX\n");
#endif
    return 0;
}

