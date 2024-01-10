#include <stdlib.h>
#include <stdio.h>

int main()
{
    char *home, *user;
    home  = getenv("HOME");
    if ( NULL != home ) {
        user = getenv("USER"); 
        if ( NULL != user )
            printf("USER=%s and HOME=%s\n", user, home);
    }
    return 0;
}
