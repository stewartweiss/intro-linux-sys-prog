include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = calc_client.c calc_server.c upcased.c upcase.c syslog_demo.c 

OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS)) spl_calc spl_calcd
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

spl_calc    : calc_client
	@mv calc_client spl_calc
	
spl_calcd   : calc_server
	@mv calc_server spl_calcd

upcased : upcased.c upcase.h
upcase  : upcase.c upcase.h
upcased1 : upcased1.c upcase1.h
upcased2: upcased2.c upcase2.h
upcaseclient1 : upcaseclient1.c upcase1.h
upcaseclient2: upcaseclient2.c upcase2.h
syslog_demo:   syslog_demo.c
