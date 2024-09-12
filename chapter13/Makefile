include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = pipe_demo1.c pipe_demo2.c  pipe_demo3.c popen_demo.c \
          shellpipe_demo.c   popen_demo.c fifosender.c fifomonitor.c \
          pipe_capacity.c 
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

pipe_demo1.o   : pipe_demo1.c   $(SPL_LIB) $(SPL_HDRS)
pipe_demo2.o   : pipe_demo2.c   $(SPL_LIB) $(SPL_HDRS)
pipe_demo3.o   : pipe_demo3.c   $(SPL_LIB) $(SPL_HDRS)
popen_demo.o   : popen_demo.c  $(SPL_LIB) $(SPL_HDRS)
shell_pipe_demo.o: shell_pipe_demo.c  $(SPL_LIB) $(SPL_HDRS)
fifomonitor.o  : fifomonitor.c fifodef.h  $(SPL_LIB) $(SPL_HDRS)
fifosender.o   : fifosender.c fifodef.h  $(SPL_LIB) $(SPL_HDRS)
pipe_capacity.o: pipe_capacity.c  $(SPL_LIB) $(SPL_HDRS)

