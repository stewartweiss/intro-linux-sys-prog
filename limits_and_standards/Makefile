# Makefile for Limits and Standards Directory 
# This Makefile uses the built-in rules for compiling C files.
# In this directory each X.c file is the source for an executable X,
# making the Makefile simpler.

# Type  make          to compile all the programs in the chapter 
#       make clean    to remove objects files and executables
#       make progname to make just progname

CC      =  /usr/bin/gcc
SRCS    = $(wildcard *.c)
OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS))
CFLAGS  =  -Wall -g -I../include
LDFLAGS = -L../lib  -lutils

.PHONY: all clean cleanall

all: $(EXECS)

clean:
	-rm -f $(OBJS) 

cleanall:
	-rm -f $(OBJS) $(EXECS)

.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@


