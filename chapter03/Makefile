# This Makefile uses the built-in rules for compiling C files.
# In this directory each X.c file is the source for an executable X,
# making the Makefile simpler.

# Type  make          to compile all the programs in the directory 
#       make clean    to remove objects files and executables
#       make progname to make just progname

CC      = /usr/bin/gcc
SRCS    = basename_demo.c cmmdargs.c envdemo.c environ_demo.c \
       get_libc_version.c getenvdemo.c gethost.c getopt_demo.c getopt_demo2.c\
       perrordemo.c printargs.c progname.c showtid.c str2num.c strerrordemo.c \
       strtoldemo.c usagecheck_demo.c
OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS))
CFLAGS  = -Wall -g 

.PHONY: all clean cleanall

all: $(EXECS)

clean:
	-rm -f $(OBJS) 

cleanall:
	-rm -f $(OBJS) $(EXECS)

.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
