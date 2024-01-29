# This Makefile uses the built-in rules for compiling C files.
# In this directory each X.c file is the source for an executable X,
# making the Makefile simpler.

# Type  make          to compile all the programs in the directory
#       make clean    to remove objects files and executables
#       make progname to make just progname

CC      = /usr/bin/gcc
SRCS    = basename_demo.c environ_demo.c get_libc_version_demo.c getenv_demo.c \
          gethostname_demo.c gettid_demo.c perror_demo.c printargs1.c  \
          printargs2.c printargs3.c progname_demo.c risky_getenv.c str2int.c \
          strerror_demo.c  strtol_demo.c usagecheck_demo.c
OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS))
CFLAGS  += -Wall -g
VPATH   =  ../include

.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	-rm -f $(OBJS)

.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
