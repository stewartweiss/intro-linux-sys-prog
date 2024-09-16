include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = spl_utmpdump.c showallusers.c makefilehole.c getpwuid_demo.c \
           spl_last.c spl_lastlog.c makelargefile.c
OBJS    := $(patsubst %.c,%.o,$(SRCS))
EXECS   := $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl

.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	-rm -f $(OBJS)


getpwuid_demo.o: getpwuid_demo.c $(SPL_LIB) $(SPL_HDRS)
makefilehole.o: makefilehole.c  $(SPL_LIB) $(SPL_HDRS)
makelargefile.o: makelargefile.c $(SPL_LIB) $(SPL_HDRS)
showallusers.o: showallusers.c  $(SPL_LIB) $(SPL_HDRS)
spl_last.o: spl_last.c      $(SPL_LIB) $(SPL_HDRS)
spl_lastlog.o: spl_lastlog.c   $(SPL_LIB) $(SPL_HDRS)
spl_utmpdump.o: spl_utmpdump.c  $(SPL_LIB) $(SPL_HDRS)
