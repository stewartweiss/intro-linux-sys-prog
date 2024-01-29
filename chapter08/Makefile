# Type  make          to compile all the programs in the directory
#       make clean    to remove objects files and executables
#       make progname to make just progname


include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = spl_du.c spl_du2.c spl_ls1.c spl_ls2.c spl_ls3.c functionptr_demo.c\
          fts_demo.c nftw_demo.c nftw_manpage_example.c scandir_demo.c \
          scandir_manpage_example.c spl_ls_rec.c spl_ls_rec2.c spl_pwd.c
OBJS    := $(patsubst %.c,%.o,$(SRCS))
EXECS   := $(patsubst %.c,%,$(SRCS))
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl
VPATH     = ../include

.PHONY: all clean cleanall

all: $(EXECS)

clean:
	-rm -f $(OBJS)

cleanall: clean
	-rm -f  $(EXECS)

dircalls.o:    common_hdrs.h
direnttest.o:  common_hdrs.h
fts_demo.o:    common_hdrs.h
functionptr_demo.o: common_hdrs.h
nftw_demo.o:   common_hdrs.h
nftw_manpage_example.o:
scandir_demo.o: common_hdrs.h
scandir_manpage_example.o:
spl_du.o:      common_hdrs.h
spl_du2.o:     common_hdrs.h
spl_ls1.o:     common_hdrs.h
spl_ls2.o:     common_hdrs.h
spl_ls3.o:     common_hdrs.h
spl_ls_rec.o:  common_hdrs.h
spl_ls_rec2.o: common_hdrs.h
spl_ls_sort.o: common_hdrs.h
spl_lsbad.o:   common_hdrs.h
spl_pwd.o:     common_hdrs.h



