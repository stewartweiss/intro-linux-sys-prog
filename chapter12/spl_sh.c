/*****************************************************************************
  Title          : spl_sh.c
  Author         : Stewart Weiss
  Created on     : May 10, 2024
  Description    : Simple example of fork(), execvp(), waitpid()
  Purpose        : To demonstrate how a simple shell can be implemented
  Usage          : spl_sh

  Build with     :

******************************************************************************
* Copyright (C) 2024 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include "common_hdrs.h"
#include <sys/wait.h>

int main( int argc, char* argv[] )
{
    char*   line = NULL;   /* Buffer to store input from user.      */
    size_t  len = 0;       /* Length of buffer.                     */
    ssize_t nread;         /* Number of bytes read by getline()     */
    char*   argvec[128];   /* Array to store command line from user */
    char*   token;         /* Used by strtok() to parse command     */
    pid_t   pid;           /* Return value from fork, child's PID   */

    do {
        printf("spl_sh$ ");                /*Print a prompt. */
        if ( -1 == (nread = getline(&line, &len, stdin )) )
            break;
        line[nread-1] = '\0';              /* Replace newline at end.       */
        token = strtok(line, " \t");       /* Parse the line using strtok() */
        int i = 0;
        while ( token != NULL ) {
            argvec[i++] = token;
            token = strtok(NULL, " \t");
        }
        argvec[i] = NULL;                  /* NULL-terminate the vector.    */
        if ( 0 == strcmp(argvec[0], "exit") )  {
            free(line);
            exit(EXIT_SUCCESS);
        }
        else if ( 0 == strcmp(argvec[0], "help") ) {
            printf("command arg arg ...\n");
            continue;
        }
        if ( (pid = fork()) == 0 )         /* Create a new process. */
            execvp(argvec[0], argvec); /* Child executes command argvec[0]. */
        else if ( -1 == pid )
           fatal_error(errno, "fork");
        else
           if ( -1 == waitpid(pid, NULL, 0))  /* Parent waits for child. */
              fatal_error(errno, "waitpid()");
        free(line);           /* Free the line allocated by getline. */
        line = NULL;
        len = 0;
    } while ( TRUE );

    return 0;
}
