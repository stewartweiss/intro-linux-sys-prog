include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = kill_demo.c raise_demo.c  restart_demo.c send_sigint.c \
          sigact_demo1.c sigact_demo2.c sigact_demo3.c  \
          signal_counter1.c signal_counter2.c signal_demo1.c signal_demo1b.c \
          signal_demo2.c signal_demo3.c signal_demo4.c \
          sigprocmask_demo1.c sigprocmask_demo2.c sigprocmask_demo3.c \
          sigsuspend_demo.c sigwait_demo1.c sigwait_demo2.c sysv_signal_demo.c
                                                                
OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl -lm

.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	\rm -f $(OBJS)

kill_demo.o : kill_demo.c $(SPL_LIB) $(SPL_HDRS)
raise_demo.o : raise_demo.c $(SPL_LIB) $(SPL_HDRS)
raise_demo1.o : raise_demo1.c $(SPL_LIB) $(SPL_HDRS)
restart_demo.o : restart_demo.c $(SPL_LIB) $(SPL_HDRS)
send_sigint.o : send_sigint.c $(SPL_LIB) $(SPL_HDRS)
sigact_demo1.o : sigact_demo1.c $(SPL_LIB) $(SPL_HDRS)
sigact_demo2.o : sigact_demo2.c $(SPL_LIB) $(SPL_HDRS)
sigact_demo3.o : sigact_demo3.c $(SPL_LIB) $(SPL_HDRS)
sigact_demo4.o : sigact_demo4.c $(SPL_LIB) $(SPL_HDRS)
signal_counter1.o : signal_counter1.c $(SPL_LIB) $(SPL_HDRS)
signal_counter2.o : signal_counter2.c $(SPL_LIB) $(SPL_HDRS)
signal_demo1.o : signal_demo1.c $(SPL_LIB) $(SPL_HDRS)
signal_demo1b.o : signal_demo1b.c $(SPL_LIB) $(SPL_HDRS)
signal_demo2.o : signal_demo2.c $(SPL_LIB) $(SPL_HDRS)
signal_demo3.o : signal_demo3.c $(SPL_LIB) $(SPL_HDRS)
signal_demo4.o : signal_demo4.c $(SPL_LIB) $(SPL_HDRS)
signal_demo5.o : signal_demo5.c $(SPL_LIB) $(SPL_HDRS)
sigprocmask_demo1.o : sigprocmask_demo1.c $(SPL_LIB) $(SPL_HDRS)
sigprocmask_demo2.o : sigprocmask_demo2.c $(SPL_LIB) $(SPL_HDRS)
sigprocmask_demo3.o : sigprocmask_demo3.c $(SPL_LIB) $(SPL_HDRS)
sigsuspend_demo.o : sigsuspend_demo.c $(SPL_LIB) $(SPL_HDRS)
sigwait_demo1.o : sigwait_demo1.c $(SPL_LIB) $(SPL_HDRS)
