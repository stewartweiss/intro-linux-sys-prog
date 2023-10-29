
#include "common_hdrs.h"
#include <dirent.h>

BOOL isdir(  const struct dirent *direntp)
{
#ifdef _DIRENT_HAVE_D_TYPE
    if( direntp->d_type  == DT_DIR)
#else
    struct stat    statbuf;
    stat(direntp->d_name, &statbuf);
    if ( S_ISDIR(statbuf.st_mode) )
#endif
        return TRUE;
    else
        return FALSE;
}


int dirsfirstsort(const struct dirent **a, const struct dirent **b)
{
    if ( isdir(*a) )
        if (! isdir(*b) )
            return -1;
        else
            return (alphasort(a,b));
    else
        if ( isdir(*b) )
            return 1;
        else
            return (alphasort(a,b));
}
