#ifndef _SHM_DEMO1_H
#define _SHM_DEMO1_H
#include "common_hdrs.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>


#define BUF_SIZE 65536
const   int offset1 = 100;
const   int offset2 = 200;

typedef struct _shared
{
    char*  ptr;
    char*  ptr2;
    char   data[4096];
}  shareddata;


#endif /* _SHM_DEMO1_H */
