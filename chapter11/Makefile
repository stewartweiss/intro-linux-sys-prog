include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = atexit_demo.c fork_demo1.c fork_demo2.c fork_demo3.c fork_demo4.c \
          fork_demo5.c sync_io_demo.c execve_demo1.c execve_demo2.c \
          print_args_env.c  spl_sh.c execl_demo.c execlp_demo.c \
          execle_demo.c execvp_demo.c wait_demo1.c wait_demo2.c wait_demo3.c \
          waitpid_demo.c waitid_demo.c  sighandler_wait_demo.c \
          sighandler_wait_demo2.c  system_demo.c
          
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

atexit_demo.o     : atexit_demo.c  $(SPL_LIB) $(SPL_HDRS)
fork_demo1.o      : fork_demo1.c   $(SPL_LIB) $(SPL_HDRS)
fork_demo2.o      : fork_demo2.c   $(SPL_LIB) $(SPL_HDRS)
fork_demo3.o      : fork_demo3.c   $(SPL_LIB) $(SPL_HDRS)
fork_demo4.o      : fork_demo4.c   $(SPL_LIB) $(SPL_HDRS)
fork_demo5.o      : fork_demo5.c   $(SPL_LIB) $(SPL_HDRS)
sync_io_demo.o    : sync_io_demo.c $(SPL_LIB) $(SPL_HDRS)
execve_demo1.o    : execve_demo1.c $(SPL_LIB) $(SPL_HDRS)
execve_demo2.o    : execve_demo2.c $(SPL_LIB) $(SPL_HDRS)
print_args_env.o  : print_args_env.c $(SPL_LIB) $(SPL_HDRS)
spl_sh.o          : spl_sh.c       $(SPL_LIB) $(SPL_HDRS)
execl_demo.o      : execl_demo.c   $(SPL_LIB) $(SPL_HDRS)
execlp_demo.o     : execlp_demo.c  $(SPL_LIB) $(SPL_HDRS)
execle_demo.o     : execle_demo.c  $(SPL_LIB) $(SPL_HDRS)
execvp_demo.o     : execvp_demo.c  $(SPL_LIB) $(SPL_HDRS)
wait_demo1.o      : wait_demo1.c   $(SPL_LIB) $(SPL_HDRS)
wait_demo2.o      : wait_demo2.c   $(SPL_LIB) $(SPL_HDRS)
wait_demo3.o      : wait_demo3.c   $(SPL_LIB) $(SPL_HDRS)
waitpid_demo.o    : waitpid_demo.c $(SPL_LIB) $(SPL_HDRS)
waitid_demo.o     : waitid_demo.c  $(SPL_LIB) $(SPL_HDRS)
sighandler_wait_demo.o : sighandler_wait_demo.c $(SPL_LIB) $(SPL_HDRS)
sighandler_wait_demo2.o: sighandler_wait_demo2.c $(SPL_LIB) $(SPL_HDRS)
system_demo.o     : system_demo.c  $(SPL_LIB) $(SPL_HDRS)


