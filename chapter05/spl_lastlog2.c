/* compile with gcc spl_lastlog2.c -o spl_lastlog2 -llastlog2 */

#include <liblastlog2/lastlog2.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <locale.h>


#define FORMAT "%c"

static int callback (const char *res_user, int64_t ll_time, const char *res_tty, const char *res_rhost, const char *res_service, const char *cb_error) {

        struct tm *bdtime;
        bdtime = localtime(&ll_time);
        char lastlog_time[64];

        if ( 0 == strftime(lastlog_time, sizeof(lastlog_time),FORMAT, bdtime))
                perror("strftime");

        printf("---------------------\n");
        printf("user: %s\n", res_user);
        printf("host: %s\n",res_rhost);
        printf("time: %s\n", lastlog_time);
        return 0;
}

int main(int argc, char *argv[]) {

        if ( setlocale(LC_ALL, "") == NULL )
                perror("setlocale");

        char *error = NULL;


        if ( 0 != ll2_read_all (NULL, callback, &error))
                perror("ll2_read_all");

        return 0;
}
