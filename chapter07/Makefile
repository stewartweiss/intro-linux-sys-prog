# Type  make          to compile all the programs in the chapter
#       make clean    to remove objects files and executables
#       make progname to make just progname

include ../Makefile.inc

CC       = /usr/bin/gcc
SRCS     = fsidtest.c lstat_manpage_example.c spl_stat.c spl_statfs.c
OBJS    := $(patsubst %.c,%.o,$(SRCS))
EXECS   := $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl
VPATH     = ../include

.PHONY: all clean cleanall

all: $(EXECS)

clean:
	-rm -f $(OBJS)

cleanall: clean
	-rm -f $(EXECS)


fsidtest.o:      common_hdrs.h
lstat_manpage_example.o:
spl_stat.o:      common_hdrs.h
spl_statfs.o:    common_hdrs.h
