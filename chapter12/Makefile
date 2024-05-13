# Type  make          to compile all the programs in the chapter
#       make clean    to remove objects files and executables
#       make progname to make just progname

include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = fork_demo1.c fork_demo2.c fork_demo3.c fork_demo4.c \
          sync_io_demo.c execve_demo1.c execve_demo2.c print_args_env.c \
          waitpid_demo.c waitid_demo.c spl_sh.c execl_demo.c execlp_demo.c \
          execle_demo.c execvp_demo.c wait_demo1.c wait_demo2.c \
          sighandler_wait_demo.c print_args_env.c
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


