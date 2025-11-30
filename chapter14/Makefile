include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = spl_calc_client.c spl_calc_server.c upcased.c upcase.c \
          syslog_demo.c 

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

spl_calc    : spl_calc_client
	@mv spl_calc_client spl_calc
	
spl_calcd   : spl_calc_server
	@mv spl_calc_server spl_calcd

upcased.o         : upcased.c upcase.h  $(SPL_LIB) $(SPL_HDRS)
upcase.o          : upcase.c upcase.h  $(SPL_LIB) $(SPL_HDRS)
spl_calc_client.o : spl_calc_client.c spl_calc.h  $(SPL_LIB) $(SPL_HDRS)
spl_calc_server.o : spl_calc_server.c spl_calc.h  $(SPL_LIB) $(SPL_HDRS)
syslog_demo.o     : syslog_demo.c  $(SPL_LIB) $(SPL_HDRS)
