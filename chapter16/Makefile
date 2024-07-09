include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = pthread_create_demo.c pthread_demo2.c pthread_detach_demo.c \
	  pthread_signal_demo.c threaded_upcased.c  pthread_getstacksize.c

OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS)) 
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl -lm -lrt -pthread
VPATH     = ../include


.PHONY: all clean cleanall

all: $(EXECS)

cleanall: clean
	-rm -f $(EXECS)

clean:
	-rm -f $(OBJS)

pthread_create_demo.o: pthread_create_demo.c
pthread_demo2.o: pthread_demo2.c
pthread_detach_demo.o: pthread_detach_demo.c
pthread_getstacksize.o: pthread_getstacksize.c
pthread_signal_demo.o: pthread_signal_demo.c
threaded_upcased.o: threaded_upcased.c
