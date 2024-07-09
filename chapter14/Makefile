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
VPATH     = ../include


.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	-rm -f $(OBJS)


pipe_demo1: pipe_demo1.c 
pipe_demo2: pipe_demo2.c 
pipe_demo3: pipe_demo3.c 
popen_demo: popen_demo.c
shell_pipe_demo: shell_pipe_demo.c
fifomonitor: fifomonitor.c fifodef.h
fifosender: fifosender.c fifodef.h
pipe_capacity: pipe_capacity.c
