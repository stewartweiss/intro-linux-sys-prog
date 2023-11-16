#include <gnu/libc-version.h>

#include <stdio.h>

void main()
{
    printf("The version of glibc is:%s\n",gnu_get_libc_version());
}
