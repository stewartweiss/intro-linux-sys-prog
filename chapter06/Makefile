# Type  make          to compile all the programs in the directory 
#       make clean    to remove objects files and executables
#       make progname to make just progname

CC      = /usr/bin/gcc
SRCS    = showutmp.c showusers.c mk_filehole.c showid.c last.c showlastlog.c
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


