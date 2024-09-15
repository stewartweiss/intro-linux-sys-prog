include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = alarm_demo1.c alarm_demo2.c alarm_demo3.c clock_nanosleep_demo.c \
          nanosleep_demo1.c nanosleep_demo2.c nanosleep_demo3.c \
          posix_timer_demo1.c posix_timer_demo2.c posix_timer_demo3.c \
          posix_timer_manpage_example.c \
          progress_bar1.c progress_bar2.c progress_bar_blocked_signal.c \
          watchfiles.c escapeseq_demo1.c escapeseq_demo2.c  clearscreen.c  \
          winsize.c
OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl -lm -lrt

.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	\rm -f $(OBJS)


alarm_demo1.o: alarm_demo1.c  $(SPL_LIB) $(SPL_HDRS)
alarm_demo2.o: alarm_demo2.c  $(SPL_LIB) $(SPL_HDRS)
alarm_demo3.o: alarm_demo3.c  $(SPL_LIB) $(SPL_HDRS)
clock_nanosleep_demo.o: clock_nanosleep_demo.c  $(SPL_LIB) $(SPL_HDRS)
clearscreen.o:  clearscreen.c $(SPL_LIB) $(SPL_HDRS)
escapeseq_demo1.o: escapeseq_demo1.c $(SPL_LIB) $(SPL_HDRS)
escapeseq_demo2.o: escapeseq_demo2.c $(SPL_LIB) $(SPL_HDRS)
nanosleep_demo1.o: nanosleep_demo1.c  $(SPL_LIB) $(SPL_HDRS)
nanosleep_demo2.o: nanosleep_demo2.c  $(SPL_LIB) $(SPL_HDRS)
nanosleep_demo3.o: nanosleep_demo3.c  $(SPL_LIB) $(SPL_HDRS)
posix_timer_demo1.o: posix_timer_demo1.c  $(SPL_LIB) $(SPL_HDRS)
posix_timer_demo2.o: posix_timer_demo2.c  $(SPL_LIB) $(SPL_HDRS)
posix_timer_demo3.o: posix_timer_demo3.c  $(SPL_LIB) $(SPL_HDRS)
posix_timer_manpage_example.o: posix_timer_manpage_example.c  $(SPL_LIB) $(SPL_HDRS)
progress_bar1.o: progress_bar1.c  $(SPL_LIB) $(SPL_HDRS)
progress_bar2.o: progress_bar2.c  $(SPL_LIB) $(SPL_HDRS)
progress_bar_blocked_signal.o: progress_bar_blocked_signal.c  $(SPL_LIB) $(SPL_HDRS)
timer_overrun_demo.o:  timer_overrun_demo.c $(SPL_LIB) $(SPL_HDRS)
watchfiles.o: watchfiles.c  $(SPL_LIB) $(SPL_HDRS)
winsize.o: winsize.c  $(SPL_LIB) $(SPL_HDRS)
