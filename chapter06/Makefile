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

fsidtest.o             : fsidtest.c       $(SPL_LIB) $(SPL_HDRS)
lstat_manpage_example.o: lstat_manpage_example.c $(SPL_LIB) $(SPL_HDRS)
spl_stat.o             : spl_stat.c       $(SPL_LIB) $(SPL_HDRS)
spl_statfs.o           : spl_statfs.c     $(SPL_LIB) $(SPL_HDRS)
