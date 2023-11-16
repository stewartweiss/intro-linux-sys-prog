#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void main()
{
    char  name[4];   /* declare string to hold returned value */
    size_t len = 3;  /* purposely too small so error is revealed */
    int   returnvalue;
    

    returnvalue =  gethostname(name, len); /* make the call */
    if ( -1 == returnvalue ) {
        printf("gethostname failed: %s\n", strerror(errno));
    }
    else
        printf("%s\n", name);
}
