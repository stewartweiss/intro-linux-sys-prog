# Type  make          to compile all the programs in the directory
#       make clean    to remove objects files and executables
#       make progname to make just progname
# This relies on the implicit rule
#    $(CC) $(LDFLAGS) x.o $(LOADLIBES) $(LDLIBS)
# to link the files into x, and  the rule
#    $(CC) -c $(CPPFLAGS) $(CFLAGS)  x.c
# to make x.o from x.c


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
VPATH     = ../include
.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	-rm -f $(OBJS)


getpwuid_demo.o: common_hdrs.h
makefilehole.o:  common_hdrs.h
makelargefile.o: common_hdrs.h
showallusers.o:  common_hdrs.h
spl_last.o:      common_hdrs.h
spl_lastlog.o:   common_hdrs.h
spl_utmpdump.o:  common_hdrs.h
