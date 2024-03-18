       #define _XOPEN_SOURCE 600
       #include <stdint.h>
       #include <stdio.h>
       #include <unistd.h>
       #include <stdlib.h>
       #include <time.h>

       int
       main(int argc, char *argv[])
       {
           clockid_t clockid;
           struct timespec ts;

           if (argc != 2) {
               fprintf(stderr, "%s <process-ID>\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           if (clock_getcpuclockid(atoi(argv[1]), &clockid) != 0) {
               perror("clock_getcpuclockid");
               exit(EXIT_FAILURE);
           }

           if (clock_gettime(clockid, &ts) == -1) {
               perror("clock_gettime");
               exit(EXIT_FAILURE);
           }

           printf("CPU-time clock for PID %s is %jd.%09ld seconds\n",
                   argv[1], (intmax_t) ts.tv_sec, ts.tv_nsec);
           exit(EXIT_SUCCESS);
       }

