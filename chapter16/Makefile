include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = pthread_prodcons.c recursive_mutex_demo.c pthread_rwlock_demo.c \
          threaded_progressbar.c vmem_usage.c
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

pthread_prodcons.o     : pthread_prodcons.c      $(SPL_LIB) $(SPL_HDRS)
recursive_mutex_demo.o : recursive_mutex_demo.c  $(SPL_LIB) $(SPL_HDRS)
pthread_rwlock_demo.o  : pthread_rwlock_demo.c   $(SPL_LIB) $(SPL_HDRS)
threaded_progressbar.o : threaded_progressbar.c  $(SPL_LIB) $(SPL_HDRS)
vmem_usage.o           : vmem_usage.c            $(SPL_LIB) $(SPL_HDRS)
