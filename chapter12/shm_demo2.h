#ifndef _SHM_DEMO2_H
#define _SHM_DEMO2_H
#include "common_hdrs.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>


#define BUF_SIZE      8192
const   int offset1 = 0x64;
const   int offset2 = 0xC8;

typedef struct _shared
{
    char*  ptr1;
    char*  ptr2;
    char   data[4096];
}  shareddata;


#endif /* _SHM_DEMO2_H */
