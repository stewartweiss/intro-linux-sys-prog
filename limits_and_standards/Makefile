# Makefile for Limits and Standards Directory 
# This Makefile uses the built-in rules for compiling C files.
# In this directory each X.c file is the source for an executable X,
# making the Makefile simpler.

# Type  make          to compile all the programs in the chapter 
#       make clean    to remove objects files and executables
#       make progname to make just progname

include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = check_features.c checklibc_v2.c iso_limits.c iso_limits2.c \
           pathmaxtest.c stdint_limits.c testlimit.c
OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl -lm
VPATH     = ../include

.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	\rm -f $(OBJS)

check_features.o: common_hdrs.h
checklibc_v2.o: common_hdrs.h
iso_limits.o: common_hdrs.h
iso_limits2.o: common_hdrs.h
pathmaxtest.o: common_hdrs.h
stdint_limits.o: common_hdrs.h
testlimit.o: common_hdrs.h
