#include <stdio.h>
#include <gnu/libc-version.h>
int main (void) { 
puts (gnu_get_libc_version ()); return 0; 
}
