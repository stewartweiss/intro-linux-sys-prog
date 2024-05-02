# Type  make          to compile all the programs in the chapter
#       make clean    to remove objects files and executables
#       make progname to make just progname

include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = kill_demo.c raise_demo.c restart_demo.c send_sigint.c \
          sigact_demo1.c sigact_demo2.c sigact_demo3.c sigact_demo4.c \
          signal_counter.c signal_counter2.c signal_demo1.c signal_demo2.c \
          signal_demo3.c signal_demo4.c signal_demo5.c sigprocmask_demo1.c \
          sigprocmask_demo2.c sigprocmask_demo3.c sigsuspendtest.c \
          sigwaittest.c sysv_signal_demo.c
OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl -lm
VPATH     = ../include

.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	\rm -f $(OBJS)


kill_demo.o: common_hdrs.h
raise_demo1.o: common_hdrs.h
restart_demo.o: common_hdrs.h
send_sigint.o: common_hdrs.h
sigact_demo1.o: common_hdrs.h
sigact_demo2.o: common_hdrs.h
sigact_demo3.o: common_hdrs.h
sigact_demo4.o: common_hdrs.h
signal_counter.o: common_hdrs.h
signal_counter2.o: common_hdrs.h
signal_demo1.o: common_hdrs.h
signal_demo2.o: common_hdrs.h
signal_demo3.o: common_hdrs.h
signal_demo4.o: common_hdrs.h
signal_demo5.o: common_hdrs.h
sigprocmask_demo1.o: common_hdrs.h
sigprocmask_demo2.o: common_hdrs.h
sigprocmask_demo3.o: common_hdrs.h
sigsuspendtest.o: common_hdrs.h
sigwaittest.o: common_hdrs.h
sysv_signal_demo.o: common_hdrs.h
