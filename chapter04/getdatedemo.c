      #define _GNU_SOURCE
       #include <time.h>
       #include <stdio.h>
       #include <stdlib.h>

       int main(int argc, char *argv[])
       {
           struct tm *tmp;

           for (int j = 1; j < argc; j++) {
               tmp = getdate(argv[j]);

               if (tmp == NULL) {
                   printf("Call %d failed; getdate_err = %d\n",
                          j, getdate_err);
                   continue;
               }

               printf("Call %d (\"%s\") succeeded:\n", j, argv[j]);
               printf("    tm_sec   = %d\n", tmp->tm_sec);
               printf("    tm_min   = %d\n", tmp->tm_min);
               printf("    tm_hour  = %d\n", tmp->tm_hour);
               printf("    tm_mday  = %d\n", tmp->tm_mday);
               printf("    tm_mon   = %d\n", tmp->tm_mon);
               printf("    tm_year  = %d\n", tmp->tm_year);
               printf("    tm_wday  = %d\n", tmp->tm_wday);
               printf("    tm_yday  = %d\n", tmp->tm_yday);
               printf("    tm_isdst = %d\n", tmp->tm_isdst);
           }
     }
