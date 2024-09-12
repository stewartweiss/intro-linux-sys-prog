include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = pthread_create_demo.c pthread_create_demo2.c pthread_detach_demo.c \
	  pthread_signal_demo.c threaded_upcased.c  pthread_getstacksize.c upcase.c
OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS)) 
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl -lm -lrt -pthread

.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	-rm -f $(OBJS)

pthread_create_demo.o  : pthread_create_demo.c   $(SPL_LIB) $(SPL_HDRS)
pthread_create_demo2.o : pthread_create_demo2.c  $(SPL_LIB) $(SPL_HDRS)
pthread_detach_demo.o  : pthread_detach_demo.c   $(SPL_LIB) $(SPL_HDRS)
pthread_getstacksize.o : pthread_getstacksize.c  $(SPL_LIB) $(SPL_HDRS)
pthread_signal_demo.o  : pthread_signal_demo.c   $(SPL_LIB) $(SPL_HDRS)
threaded_upcased.o     : threaded_upcased.c      $(SPL_LIB) $(SPL_HDRS)
upcase.o               : upcase.c                $(SPL_LIB) $(SPL_HDRS)
