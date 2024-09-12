include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = curses_version.c getstr_demo.c curses_demo1.c tiled_windows.c sprite_curses.c \
          sprite.c  wintest.c  mintime_test_demo.c spl_top.c
OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl -lm -lrt -lncurses

.PHONY: all clean cleanall

all: $(EXECS) 

cleanall: clean
	-rm -f $(EXECS)

clean:
	-rm -f $(OBJS)

spl_top: spl_top.o top_utils.o top_utils.h ps_utils.c ps_utils.h \
                  $(SPL_LIB)  $(SPL_HDRS)
	$(CC)  $(CFLAGS) $(CPPFLAGS) -o spl_top top_utils.o spl_top.c  \
           $(LDFLAGS) $(LDLIBS)

$(EXECS): -lspl

getstr_demo.o: getstr_demo.c $(SPL_LIB)  $(SPL_HDRS)
curses_demo1.o: curses_demo1.c $(SPL_LIB)  $(SPL_HDRS)
curses_version.o: curses_version.c $(SPL_LIB)  $(SPL_HDRS)
tiled_windows.o: tiled_windows.c  $(SPL_LIB)  $(SPL_HDRS)
top_utils.o: top_utils.c  top_utils.h
sprite_curses.o: sprite_curses.c $(SPL_LIB)  $(SPL_HDRS)
mintime_test_demo.o: mintime_test_demo.c $(SPL_LIB) $(SPL_HDRS)
sprite.o: sprite.c  $(SPL_LIB) $(SPL_HDRS)
