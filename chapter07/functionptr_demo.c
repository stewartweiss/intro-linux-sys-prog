/****************************************************************************
  Title          : functionptr_demo.c
  Author         : Stewart Weiss
  Created on     : December 29, 2023
  Description    : Shows how to use function pointer parameters.
  Usage          : functionptr_demo
                   where files may be any file type including directories
  Build with     : gcc -Wall -g -I ../include functionptr_demo.c \
                     -o functionptr_demo -L../lib -lspl

******************************************************************************
* Copyright (C) 2023 - Stewart Weiss                                         *
*                                                                            *
* This code is free software; you can use, modify, and redistribute it       *
* under the terms of the GNU General Public License as published by the      *
* Free Software Foundation; either version 3 of the License, or (at your     *
* option) any later version. This code is distributed WITHOUT ANY WARRANTY;  *
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the file COPYING.gplv3 for details.                *
*****************************************************************************/

#include "common_hdrs.h"

/* Declare funcp to be the name of a type that is a pointer to
   a function that returns int and expects two int arguments. */
typedef int (*funcp)(int,int);


/* Define a function g() whose form matches the funcp pattern. */
int g(int a, int b) { return a*b;}

/* Define a function h() whose form also matches the funcp pattern. */
int h(int a, int b) { return a+b;}


/* Declare a function f() that expects in its first parameter any function
   whose prototype matches funcp and then two ints after it. f() just
   applies the function passed to it on the two integers. */
int f(funcp q, int x, int y)
{
    int res = q(x,y);
    return res;
}

int main(int argc, char* argv[])
{
    funcp two_op_function;
    two_op_function = &g;
    printf("2 times 3 = %d\n", f(two_op_function,2,3));
    two_op_function = h;
    printf("4 plus 5  = %d\n", f(two_op_function,4,5));
    return 0;
}
