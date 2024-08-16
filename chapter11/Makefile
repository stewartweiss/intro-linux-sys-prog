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
alarm_demo2.o: common_hdrs.h
clock_nanosleep_demo.o: common_hdrs.h
nanosleep_demo1.o: common_hdrs.h
nanosleep_demo2.o: common_hdrs.h
nanosleep_demo3.o: common_hdrs.h
posix_timer_demo1.o: common_hdrs.h
posix_timer_demo2.o: common_hdrs.h
posix_timer_demo3.o: common_hdrs.h
posix_timer_manpage_example.o: common_hdrs.h
progress_bar1.o: common_hdrs.h
progress_bar2.o: common_hdrs.h
progress_bar_blocked_signal.o: common_hdrs.h
watchfiles.o: common_hdrs.h
