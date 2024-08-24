include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = ancestors.c displayvm.c hello_world.c memlayout.c showbreak.c \
          print_elfhdrs.c print_elfphdr.c
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
	\rm -f $(OBJS)


alarm_demo1.o: common_hdrs.h
displayvm.o:   common_hdrs.h
elf_utils.o:   common_hdrs.h
memlayout1.o:  common_hdrs.h
showbreak.o:   common_hdrs.h
spl_ps.o:      common_hdrs.h ps_utils.h ../common/ps_utils.c

