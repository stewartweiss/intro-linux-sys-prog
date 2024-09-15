include ../Makefile.inc

CC        = /usr/bin/gcc
SRCS      = spl_cp1.c spl_cp2.c spl_libcalloverhead.c spl_syscalloverhead.c
OBJS     := $(patsubst %.c,%.o,$(SRCS))
EXECS    := $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl

.PHONY: all clean cleanall

all: $(EXECS)


cleanall: clean
	-rm -f  $(EXECS)

clean:
	-rm -f $(OBJS)


spl_cp1.o: spl_cp1.c $(SPL_LIB) $(SPL_HDRS)
spl_cp2.o: spl_cp2.c $(SPL_LIB) $(SPL_HDRS)
spl_libcalloverhead.o: spl_libcalloverhead.c $(SPL_LIB) $(SPL_HDRS)
spl_syscalloverhead.o: spl_syscalloverhead.c $(SPL_LIB) $(SPL_HDRS)
