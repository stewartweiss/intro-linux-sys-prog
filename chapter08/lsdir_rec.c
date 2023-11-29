#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#include "common_hdrs.h"
#include "dir_utils.h"

void listdir( DIR *dirp, char *dirname,  int flags )
{
    struct dirent *direntp;       /* Pointer to directory entry structure  */
    BOOL          done = FALSE;   /* Flag to control loop execution        */
    char          pathname[PATH_MAX];  /* Pathname of file to open         */
    DIR           *subdirp;       /* Dir stream for subdirectory           */

    while ( !done ) {
        errno = 0;
        direntp = readdir( dirp );                       /* Get next entry. */
        if ( direntp == NULL && errno != 0 )
            /* Not the end of the stream, but an error from readdir()       */
            perror("readdir");
        else if ( direntp == NULL )
            /* Implies errno == 0, so it's the end of the stream.           */
           done = TRUE;
        else {
            if (strcmp(direntp->d_name, ".") != 0
             && strcmp(direntp->d_name, "..") != 0) {
                sprintf(pathname, "%s/%s", dirname, direntp->d_name);
                printf("%s\n",pathname);
                if ( isdir(direntp) ) {
                    errno = 0;
                    if ( ( subdirp = opendir(pathname) ) == NULL ) {
                        error_mssge(errno, direntp->d_name);
                    }
                    else {
                        listdir(subdirp, pathname, flags);
                        closedir(subdirp);
                    }
                }
            }
        }
    }
    printf("\n");
}




int main(int argc, char *argv[])
{
    DIR   *dirp;
    int    i;
    int    ls_flags = 0;

    if ( 1 == argc ) {      /* No arguments; use current working directory. */
        errno = 0;
        if ( ( dirp = opendir(".") ) == NULL )
            fatal_error(errno, "opendir");           /* Could not open cwd. */
        else
           listdir( dirp, ".",  ls_flags );
    }
    else {         /* For each command-line argument, call opendir() on it. */
        for ( i = 1; i < argc; i++) {
            errno = 0;
            if ( ( dirp = opendir(argv[i]) ) == NULL ) {
                if ( errno == ENOTDIR )  /* It's not a directory. */
                    printf("%s\n", argv[i] );
                else  /* It's an error. */
                    error_mssge(errno, argv[i]);
            }
            else {  /* A successful open of a directory */
                printf("\n%s:\n", argv[i] );
                listdir( dirp, argv[i], ls_flags);
                closedir(dirp);
            }
        }
    }
    return 0;
}

