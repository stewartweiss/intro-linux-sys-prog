#include "common_hdrs.h"
#include <stdint.h>

#define CLOCKID CLOCK_MONOTONIC



void handler(int sig, siginfo_t *si, void *uc)
{
    timer_t  timerid =  *(timer_t*)(si->si_value.sival_ptr);
    printf(" %ju ", (uintmax_t) timerid);
    fflush(stdout);

}


int  main(int argc, char *argv[])
{
    timer_t timerid[2];
    struct sigevent sev;
    struct itimerspec timer_setting[2];
    long long freq_nanosecs;
    struct sigaction sa;
    char c;
    int nbytes;


    /* Install signal handler for SIGUSR1. */
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;

    if (-1 == sigaction(SIGUSR1, &sa, NULL) )
        fatal_error(errno, "sigaction");

    if (-1 == sigaction(SIGUSR2, &sa, NULL) )
        fatal_error(errno, "sigaction");

    /* Create the timers. */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    sev.sigev_value.sival_ptr = &(timerid[0]);
    if (timer_create(CLOCKID, &sev, &(timerid[0])) == -1)
       fatal_error(errno, "timer_create");
    printf("First timer ID is %ju\n", (uintmax_t) timerid[0]);

    sev.sigev_signo = SIGUSR2;
    sev.sigev_value.sival_ptr = &(timerid[1]);
    if (timer_create(CLOCKID, &sev, &(timerid[1])) == -1)
       fatal_error(errno, "timer_create");
    printf("Second timer ID is %ju\n", (uintmax_t) timerid[1]);


    /* Ser the intervals for the two timers. */
    freq_nanosecs = 1400000000;
    timer_setting[0].it_value.tv_sec = freq_nanosecs / 1000000000;
    timer_setting[0].it_value.tv_nsec = freq_nanosecs % 1000000000;
    timer_setting[0].it_interval.tv_sec = timer_setting[0].it_value.tv_sec;
    timer_setting[0].it_interval.tv_nsec = timer_setting[0].it_value.tv_nsec;

    freq_nanosecs = 666666667;
    timer_setting[1].it_value.tv_sec = freq_nanosecs / 1000000000;
    timer_setting[1].it_value.tv_nsec = freq_nanosecs % 1000000000;
    timer_setting[1].it_interval.tv_sec = timer_setting[1].it_value.tv_sec;
    timer_setting[1].it_interval.tv_nsec = timer_setting[1].it_value.tv_nsec;

    if (timer_settime(timerid[0], 0, &(timer_setting[0]), NULL) == -1)
        fatal_error(errno, "timer_settime");

    if (timer_settime(timerid[1], 0, &(timer_setting[1]), NULL) == -1)
        fatal_error(errno, "timer_settime");

    printf("Enter 'q' to terminate this program.\n");
    while ( TRUE ) {
        if ( -1 == (nbytes = read(STDIN_FILENO, &c, 1)) )
            fatal_error(errno, "read");
        else if ( c == 'q' )
            break;
    }
    exit(EXIT_SUCCESS);
}

