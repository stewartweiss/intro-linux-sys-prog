/*****************************************************************************
  Title          :
  Author         : Stewart Weiss
  Created on     :
  Description/   :
  Purpose        :
  Usage          :
  Build with     :

******************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/#include <unistd.h>
#include <stdio.h>

extern char etext, edata, end;

int main(int argc, char *argv[])
{
    void *break_location = sbrk(0);
    printf("Location of end           = %10p\n"
           "Location of program break = %10p\n", &end, break_location);
    printf("Difference = %ld\n", (long) (break_location - (void*) &end) );
    pause();
    return 0;
}



