# Type  make          to compile all the programs in the chapter 
#       make clean    to remove objects files and executables
#       make progname to make just progname

include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = fsidtest.c lstat_manpage_example.c showstat.c showstatfs.c
OBJS    := $(patsubst %.c,%.o,$(SRCS))
EXECS   := $(patsubst %.c,%,$(SRCS))
CFLAGS  += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g -I${SPL_INCLUDE_DIR}
LDFLAGS += -L ${SPL_LIB_DIR} -lspl

.PHONY: all clean cleanall

all: $(EXECS)

clean:
	-rm -f $(OBJS) 

cleanall:
	-rm -f $(OBJS) $(EXECS)

.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
