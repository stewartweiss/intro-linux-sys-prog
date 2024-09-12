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

.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	-rm -f $(OBJS)

nonblock_demo1.o : nonblock_demo1.c  $(SPL_LIB) $(SPL_HDRS)
nonblock_demo2.o : nonblock_demo2.c  $(SPL_LIB) $(SPL_HDRS)
sigio_counter.o  : sigio_counter.c   $(SPL_LIB) $(SPL_HDRS)
sigio_demo.o     : sigio_demo.c      $(SPL_LIB) $(SPL_HDRS)
aio_write_demo.o : aio_write_demo.c  $(SPL_LIB) $(SPL_HDRS)
select_demo.o    : select_demo.c     $(SPL_LIB) $(SPL_HDRS)
