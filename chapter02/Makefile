include ../Makefile.inc

CC       = /usr/bin/gcc
SRCS    = basename_demo.c environ_demo.c get_libc_version_demo.c getenv_demo.c \
          gethostname_demo.c gettid_demo.c perror_demo.c printargs1.c  \
          printargs2.c printargs3.c progname_demo.c risky_getenv.c str2int.c \
          strerror_demo.c  strtol_demo.c usagecheck_demo.c getopt_demo.c
OBJS    := $(patsubst %.c,%.o,$(SRCS))
EXECS   := $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl

.PHONY: all clean cleanall

all: $(EXECS)

clean:
	-rm -f $(OBJS)

cleanall: clean
	-rm -f $(EXECS)


basename_demo.o: basename_demo.c  $(SPL_LIB) $(SPL_HDRS)
environ_demo.o: environ_demo.c  $(SPL_LIB) $(SPL_HDRS)
get_libc_version_demo.o: get_libc_version_demo.c  $(SPL_LIB) $(SPL_HDRS)
getenv_demo.o: getenv_demo.c  $(SPL_LIB) $(SPL_HDRS)
gethostname_demo.o: gethostname_demo.c  $(SPL_LIB) $(SPL_HDRS)
getopt_demo.c: $(SPL_LIB) $(SPL_HDRS)
gettid_demo.o: gettid_demo.c  $(SPL_LIB) $(SPL_HDRS)
perror_demo.o: perror_demo.c  $(SPL_LIB) $(SPL_HDRS)
printargs1.o: printargs1.c  $(SPL_LIB) $(SPL_HDRS)
printargs2.o: printargs2.c  $(SPL_LIB) $(SPL_HDRS)
printargs3.o: printargs3.c  $(SPL_LIB) $(SPL_HDRS)
progname_demo.o: progname_demo.c  $(SPL_LIB) $(SPL_HDRS)
risky_getenv.o: risky_getenv.c  $(SPL_LIB) $(SPL_HDRS)
str2int.o: str2int.c  $(SPL_LIB) $(SPL_HDRS)
strerror_demo.o: strerror_demo.c  $(SPL_LIB) $(SPL_HDRS)
strtol_demo.o: strtol_demo.c  $(SPL_LIB) $(SPL_HDRS)
usagecheck_demo.o: usagecheck_demo.c  $(SPL_LIB) $(SPL_HDRS)
