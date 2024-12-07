#ifndef _SHM_DEMO1_H
#define _SHM_DEMO1_H
#include "common_hdrs.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>


#define BUF_SIZE      8192
const   int offset1 = 0x64;
const   int offset2 = 0xC8;

#endif /* _SHM_DEMO1_H */
