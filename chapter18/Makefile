include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    =  nonblock_demo1.c nonblock_demo2.c sigio_counter.c sigio_demo.c \
           aio_write_demo.c cp_aio.c select_demo.c 
           

OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl -lm -lrt 
VPATH     = ../include


.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	-rm -f $(OBJS)

nonblock_demo1.o : nonblock_demo1.c
nonblock_demo2.o : nonblock_demo2.c
sigio_counter.o  : sigio_counter.c
sigio_demo.o     : sigio_demo.c
aio_write_demo.o : aio_write_demo.c
select_demo.o    : select_demo.c
