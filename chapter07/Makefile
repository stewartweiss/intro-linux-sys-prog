include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = dircalls.c direnttest.c fts_demo.c functionptr_demo.c nftw_demo.c\
          nftw_manpage_example.c scandir_demo.c scandir_manpage_example.c \
          spl_du1.c spl_du2.c spl_ls1.c spl_ls2.c spl_ls3.c spl_ls_rec1.c \
          spl_ls_rec2.c  spl_lsbad.c spl_pwd.c testdircalls.c
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


dircalls.o: dircalls.c $(SPL_LIB) $(SPL_HDRS)
direnttest.o: direnttest.c $(SPL_LIB) $(SPL_HDRS)
fts_demo.o: fts_demo.c $(SPL_LIB) $(SPL_HDRS)
functionptr_demo.o: functionptr_demo.c $(SPL_LIB) $(SPL_HDRS)
nftw_demo.o: nftw_demo.c $(SPL_LIB) $(SPL_HDRS)
nftw_manpage_example.o: nftw_manpage_example.c $(SPL_LIB) $(SPL_HDRS)
scandir_demo.o: scandir_demo.c $(SPL_LIB) $(SPL_HDRS)
scandir_manpage_example.o: scandir_manpage_example.c $(SPL_LIB) $(SPL_HDRS)
spl_du1.o: spl_du1.c $(SPL_LIB) $(SPL_HDRS)
spl_du2.o: spl_du2.c $(SPL_LIB) $(SPL_HDRS)
spl_ls1.o: spl_ls1.c $(SPL_LIB) $(SPL_HDRS)
spl_ls2.o: spl_ls2.c $(SPL_LIB) $(SPL_HDRS)
spl_ls3.o: spl_ls3.c $(SPL_LIB) $(SPL_HDRS)
spl_ls_rec1.o: spl_ls_rec1.c $(SPL_LIB) $(SPL_HDRS)
spl_ls_rec2.o: spl_ls_rec2.c $(SPL_LIB) $(SPL_HDRS)
spl_lsbad.o: spl_lsbad.c $(SPL_LIB) $(SPL_HDRS)
spl_pwd.o: spl_pwd.c $(SPL_LIB) $(SPL_HDRS)
testdircalls.o: testdircalls.c $(SPL_LIB) $(SPL_HDRS)


