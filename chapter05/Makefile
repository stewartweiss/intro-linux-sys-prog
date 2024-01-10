include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = spl_copy.c spl_copy2.c spl_libcalloverhead.c spl_syscalloverhead.c
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

cp1:  cp1.o
	$(CC) $(CFLAGS) cp1.o  $(LDFLAGS) -o $@


