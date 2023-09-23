# Type  make          to compile all the programs in the directory 
#       make clean    to remove objects files and executables
#       make progname to make just progname

CC      = /usr/bin/gcc
SRCS    = copy.c copy2.c libcalloverhead.c syscalloverhead.c
OBJS    := $(patsubst %.c,%.o,$(SRCS))
EXECS   := $(patsubst %.c,%,$(SRCS))
CFLAGS  +=  -Wall -g -I ../include
LDFLAGS +=  -L../lib -lspl
LDFLAGS += -L ../lib -lspl

.PHONY: all clean cleanall

all: $(EXECS)

clean:
	-rm -f $(OBJS) 

cleanall:
	-rm -f $(OBJS) $(EXECS)

.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@

cp1:  cp1.o
	$(CC) $(CFLAGS) cp1.o  $(LDFLAGS) -o $@


