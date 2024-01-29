# This relies on the implicit rule
#    $(CC) $(LDFLAGS) x.o $(LOADLIBES) $(LDLIBS)
# to link the files into x, and  the rule
#    $(CC) -c $(CPPFLAGS) $(CFLAGS)  x.c
# to make x.o from x.c


include ../Makefile.inc

CC        = /usr/bin/gcc
SRCS      = spl_cp.c spl_cp2.c spl_libcalloverhead.c spl_syscalloverhead.c
OBJS     := $(patsubst %.c,%.o,$(SRCS))
EXECS    := $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl
VPATH     = ../include

.PHONY: all clean cleanall

all: $(EXECS)


cleanall: clean
	-rm -f  $(EXECS)

clean:
	-rm -f $(OBJS)


spl_cp.o: common_hdrs.h
spl_cp2.o: common_hdrs.h
spl_libcalloverhead.o: common_hdrs.h
spl_syscalloverhead.o: common_hdrs.h
