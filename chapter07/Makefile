# Makefile for Chapter 03
# Revised   Sept. 22, 2006
# Type  make          to compile all the programs in the chapter 
#       make clean    to remove objects files and executables
#       make progname to make just progname

CC      = /usr/bin/gcc
SRCS    = fsidtest.c lstat_manpage_example.c showstat.c showstatfs.c
OBJS    := $(patsubst %.c,%.o,$(SRCS))
EXECS   := $(patsubst %.c,%,$(SRCS))
CFLAGS  +=  -Wall -g -I ../include
LDFLAGS +=  -L../lib -lspl

.PHONY: all clean cleanall

all: $(EXECS)

clean:
	-rm -f $(OBJS) 

cleanall:
	-rm -f $(OBJS) $(EXECS)

.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
