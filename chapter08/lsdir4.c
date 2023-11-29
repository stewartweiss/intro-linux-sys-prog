#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#include "common_hdrs.h"
#include "dir_utils.h"


int scan_one_dir(const char* dirname )
{
    struct dirent **namelist;
    int i, n;
    char   pathname[PATH_MAX];

    errno = 0;
    if ( (n = scandir(dirname, &namelist, NULL, alphasort) ) < 0){
        fatal_error(errno, "scandir");
    }
    for (i = 0; i < n; i++) {
        if (strcmp(namelist[i]->d_name, ".") != 0
            && strcmp(namelist[i]->d_name, "..") != 0) {
            printf("%s/%s\n",dirname,namelist[i]->d_name);
            if ( isdir(namelist[i]) ) {
                sprintf(pathname, "%s/%s", dirname, namelist[i]->d_name);
                scan_one_dir(pathname);
            }
        }
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
        scan_one_dir(".");
    else {
        int i;
        for (i = 1; i < argc; i++ ) {
            printf("%s:\n", argv[i]);
            scan_one_dir(argv[i]);
            if ( i < argc-1 ) printf("\n");
        }
    }
    exit(EXIT_SUCCESS);
}

