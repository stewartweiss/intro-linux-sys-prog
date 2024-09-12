#include "common_hdrs.h"
#include <sys/wait.h>


/*
 * Demonstration of Pipes
 *
 * Author: Stewart Weiss
 *
 * Date: Oct. 25, 2000
 *
 * Modified Oct. 21, 2002
 *  > I simplified how the child process write to the screen.
 *
 * This program demonstrates how the UNIX pipe() call works.
 * The process creates a pipe and immediately forks a child.
 * The child process closes the write end of the pipe, creates
 * an 11-character buffer for reading, and then enters a loop
 * in which it repeatedly reads from the read-end of the pipe
 * and displays them on standard output until there are no
 * characters left.
 *
 * The child appends a null character to the data it receives
 * and uses the printf facility to print to the console.
 *
 * The parent creates an array of variable-size strings and sends each
 * string down the pipe one after the other. Notice that the parent is
 * writing in larger chunks than the child reads. This demonstrates that
 * the two processes do not need to read and write equal size blocks of data.
 *
 * I have made the child sleep a bit to give the parent a chance to display
 * its message on the console first.  I then make the parent sleep to show
 * that the child will block waiting for input in the pipe.
 *
 * I have also shown how the child returns its exit status to the parent and
 * how the parent retrieves it. Remember that wait(&status) on normal exit
 * will place the exit status in the high-order 8 bits of status, and that
 * the low-order bits will be zero if and only if there is no error.
 *
 * Interspersed through the program are various messages to show who is doing
 * what, and when. Notice that these messages do not necessarily come out on
 * the console in a particular order, unless you synchronize the processes.
 */
int fillpipe( int pipefd )
{
    int count = 0;
    int flagset;
    int nbytes = 0;

    char fill[PIPE_BUF/2];
    memset(fill, '0', PIPE_BUF/2);

    flagset   = fcntl(pipefd, F_GETFL);
    fcntl(pipefd, F_SETFL, flagset | O_NONBLOCK);
    while ( count < 61000 ) {
        if ( (nbytes = write(pipefd, fill, PIPE_BUF/2) ) <= 0) {
            if ( errno != EAGAIN )
                fatal_error(errno, "write to pipe");
            break;
        }
        count += nbytes;
    }
    flagset   = fcntl(pipefd, F_GETFL);
    fcntl(pipefd, F_SETFL, flagset & ~O_NONBLOCK);
    return count;
}



int child( int readfd );
/* This function encapsulates the child's code. Normally, */
/* the child would be exec-ed and would inherit the parent's open */
/* file dscriptors. I pass it in instead to simplify matters. */


int main(int argc, char* argv[])
{
    int pid;
    int fd[2];
    printf("Starting up...\n");
    if ( -1 == pipe(fd))
    {
        perror("pipe call");
        exit(1);
    }

    if ( 0 == (pid = fork())) {
        int result;
        close(fd[1]);
        result = child(fd[0]);  /* pass the read-end of the descriptor */
        exit(result);
    }
    else {  /* parent code */
        int status;        /* for retrieving exit status of child */
        int length;        /* length of string written to pipe */
        int i;
        char *msg[4];      /* array of strings to write to pipe */
        printf("Parent: I am the parent of process %d and my pid is %d.\n",
                pid,getpid());

        /* now delay 5 seconds to show that child will wait for message */
        sleep(5);
        close(fd[0]);      /* close the read end of the pipe */
        int s = fillpipe(fd[1]);
        printf("Pipe has %d bytes in it.\n", s);
        msg[0] = "If a packet hits a pocket on a socket on a port\n";
        msg[1] = "and the bus is interrupted as a very last resort,\n";
        msg[2] = "and the address of the memory makes your floppy disk abort,\n";
        msg[3] = "then the socket packet pocket has an error to report.\n";

        for (i = 0; i < 4; i++) {
            length = strlen( msg[i]);
            if ( -1 == write(fd[1], msg[i], length))
                printf("Parent cannot write all data into pipe.\n");
        }

        printf("\nParent: Finished sending message.\n");
        close(fd[1]);

        if ((pid = wait(&status)) == -1)
        {
            perror("wait failed");
            exit(2);
        }
        if ((status & 0x000000FF) ==0) {
            status = status >> 8;
            printf("Parent: Child %d exited with status %d.\n", pid, status);
        }
        else {
            printf("Parent: Child %d exited with error code %d.\n",pid,
                    status%256);
        }

    }

    return 0;
}


int child( int readfd /*, int writefd*/ )
{
    const int BUFSIZE = 11;
    int pid;
    char buf[BUFSIZE];
    int bytesread;
    /* give parent the head start */
    pid = getpid();

    printf("Child: I am the child. My pid is %d and parent's pid is %d.\n",
            pid, getppid());
    sleep(5);
    printf("Child: I have received the following message:\n\n");

    while ( 0 != (bytesread = read(readfd, buf, BUFSIZE-1))) {
          buf[bytesread] = '\0';
          printf("%s",  buf);
    }
    printf("\nChild: End of message.\n");
    close(readfd);
    printf("Child: I am now sending a status of 4 to my parent.\n");
    return 4;
}


