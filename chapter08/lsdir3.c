#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#include "common_hdrs.h"
#include "dir_utils.h"   /* Needed for isdir() */


/** dirsfirstsort(&d1, &d2) compares the direct structures pointed to by
*   d1 and d2 and returns -1, 0, or 1 according to the sort order:
*   if both are directories, return
*       -1 if d1 <= d2 by the string collating order
*        1 if d2 > d1 by the string collating order
*   else
*       -1  if d1 is a directory and d2 is not
*        1  if d1 is not a directory and d2 is
*   else if neither is a directory, return -1, 0, or 1 if d1 < d2,
*         d1 == d2, or d1 > d2 respectively.
*/
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

void print( const struct dirent* direntp )
{
        printf("%s", direntp->d_name);
        if ( isdir(direntp) )
            printf("/");
        printf("\n");
}


int scan_one_dir(const char* dirname, void (*process )(const struct dirent* ))
{
    struct dirent **namelist;
    int n;

    errno = 0;
    if ( (n = scandir(dirname, &namelist, NULL, dirsfirstsort) ) < 0){
        fatal_error(errno, "scandir");
    }

    int i;
    for (i = 0; i < n; i++) {
        process(namelist[i]);
        free(namelist[i]);
    }
    free(namelist);
    return(EXIT_SUCCESS);
}


int main(int argc, char* argv[])
{

    if ( setlocale(LC_TIME, "")  == NULL )
        fatal_error( LOCALE_ERROR,
                 "setlocale() could not set the given locale");

    if ( 1 == argc )
        scan_one_dir(".", print);
    else {
        int i;
        for (i = 1; i < argc; i++ ) {
            printf("%s:\n", argv[i]);
            scan_one_dir(argv[i], print);
            if ( i < argc-1 ) printf("\n");
        }
    }
    exit(EXIT_SUCCESS);
}

