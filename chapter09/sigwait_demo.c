#define _GNU_SOURCE
#include "common_hdrs.h"
#include <signal.h>

void  handler(int signum)
{
    printf("Exiting because of SIGQUIT\n");
    raise(SIGTERM);
}


int main(void)
{
    struct sigaction  sigact;
    sigset_t mask;
    int sig;

    sigact.sa_handler = handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags  = 0;
    sigaction(SIGQUIT, &sigact, NULL);
    printf("PID=%d\n", getpid());

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGUSR1);
    if ( -1 == sigprocmask(SIG_BLOCK, &mask, NULL) )
        fatal_error(errno, "sigprocmask");

    while (TRUE) {
        if ( sigwait(&mask, &sig) != 0 )
             fatal_error(errno, "sigwait");
        switch ( sig ) {
        case SIGINT: printf("SIGINT received\n"); break;
        case SIGHUP: printf("SIGHUP received\n"); break;
        default:
             printf("Received some other signal.\n");
        }
    }
    return 0;
}
