CC      = /usr/bin/gcc
SRCS    = bad_getenv_demo.c getenv_demo.c getpid_demo.c gettid_demo.c hello.c \
          hello_world.c stdin_demo.c
OBJS    = $(patsubst %.c,%.o,$(SRCS))
EXECS   = $(patsubst %.c,%,$(SRCS))
CFLAGS  = -Wall -g 

.PHONY: all clean cleanall

all: $(EXECS)

clean:
	-rm -f $(OBJS) 

cleanall:
	-rm -f $(OBJS) $(EXECS)

.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@
