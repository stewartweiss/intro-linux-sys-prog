#include "common_hdrs.h"
#include <stdint.h>

#define CLOCKID CLOCK_MONOTONIC

void handler0(int sig, siginfo_t *si, void *uc)
{
    char  output[] = "\n";
    write(STDOUT_FILENO, output, strlen(output));
    fflush(stdout);
}

void handler1(int sig, siginfo_t *si, void *uc)
{
    char  output[] = "\r       1";
    write(STDOUT_FILENO, output, strlen(output));
    fflush(stdout);
}

void handler2(int sig, siginfo_t *si, void *uc)
{
    char  output[] = "\r              2";
    write(STDOUT_FILENO, output, strlen(output));
    fflush(stdout);
}

void handler3(int sig, siginfo_t *si, void *uc)
{
    static BOOL toggle = TRUE;
    char  output1[] = "\r                       3";
    char  output2[] = "\r                        ";
    if ( toggle ) {
        write(STDOUT_FILENO, output1, strlen(output1));
        toggle = FALSE;
    }
    else {
        write(STDOUT_FILENO, output2, strlen(output2));
        toggle = TRUE;
    }
    fflush(stdout);
}


int  main(int argc, char *argv[])
{
    timer_t timerid[4];
    struct sigevent sev;
    struct itimerspec timer_setting[4];
    struct sigaction sa;
    char c;
    int i, nbytes;


    /* Install signal handlers. */
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigemptyset(&sa.sa_mask);

    sa.sa_sigaction = handler0;
    if (-1 == sigaction(SIGRTMIN, &sa, NULL) )
        fatal_error(errno, "sigaction");

    sa.sa_sigaction = handler1;
    if (-1 == sigaction(SIGRTMIN+1, &sa, NULL) )
        fatal_error(errno, "sigaction");

    sa.sa_sigaction = handler2;
    if (-1 == sigaction(SIGRTMIN+2, &sa, NULL) )
        fatal_error(errno, "sigaction");

    sa.sa_sigaction = handler3;
    if (-1 == sigaction(SIGRTMIN+3, &sa, NULL) )
        fatal_error(errno, "sigaction");


    /* Create the timers. */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    if (timer_create(CLOCKID, &sev, &(timerid[0])) == -1)
       fatal_error(errno, "timer_create");

    sev.sigev_signo = SIGRTMIN+1;
    if (timer_create(CLOCKID, &sev, &(timerid[1])) == -1)
       fatal_error(errno, "timer_create");

    sev.sigev_signo = SIGRTMIN+2;
    if (timer_create(CLOCKID, &sev, &(timerid[2])) == -1)
       fatal_error(errno, "timer_create");

    sev.sigev_signo = SIGRTMIN+3;
    if (timer_create(CLOCKID, &sev, &(timerid[3])) == -1)
       fatal_error(errno, "timer_create");


    /* Set the intervals for the timers. */
    timer_setting[0].it_value.tv_sec = 0;
    timer_setting[0].it_value.tv_nsec = 250000000;
    timer_setting[0].it_interval.tv_sec = timer_setting[0].it_value.tv_sec;
    timer_setting[0].it_interval.tv_nsec = timer_setting[0].it_value.tv_nsec;

    timer_setting[1].it_value.tv_sec = 0;
    timer_setting[1].it_value.tv_nsec = 750000000;
    timer_setting[1].it_interval.tv_sec = timer_setting[1].it_value.tv_sec;
    timer_setting[1].it_interval.tv_nsec = timer_setting[1].it_value.tv_nsec;

    timer_setting[2].it_value.tv_sec = 1;
    timer_setting[2].it_value.tv_nsec = 250000000;
    timer_setting[2].it_interval.tv_sec = timer_setting[2].it_value.tv_sec;
    timer_setting[2].it_interval.tv_nsec = timer_setting[2].it_value.tv_nsec;

    timer_setting[3].it_value.tv_sec = 1;
    timer_setting[3].it_value.tv_nsec = 750000000;
    timer_setting[3].it_interval.tv_sec = timer_setting[3].it_value.tv_sec;
    timer_setting[3].it_interval.tv_nsec = timer_setting[3].it_value.tv_nsec;

    for ( i = 0; i < 4; i++ )
        if (timer_settime(timerid[i], 0, &(timer_setting[i]), NULL) == -1)
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

