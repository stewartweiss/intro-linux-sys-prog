include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = calc_client.c calc_server.c upcased.c upcase.c syslog_demo.c 

OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS)) spl_calc spl_calcd
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

spl_calc    : calc_client
	@mv calc_client spl_calc
	
spl_calcd   : calc_server
	@mv calc_server spl_calcd

upcased.o       : upcased.c upcase.h  $(SPL_LIB) $(SPL_HDRS)
upcase.o        : upcase.c upcase.h  $(SPL_LIB) $(SPL_HDRS)
calc_client.o   : calc_client.c spl_calc.h  $(SPL_LIB) $(SPL_HDRS)
calc_server.o   : calc_server.c spl_calc.h  $(SPL_LIB) $(SPL_HDRS)
syslog_demo.o   : syslog_demo.c  $(SPL_LIB) $(SPL_HDRS)
