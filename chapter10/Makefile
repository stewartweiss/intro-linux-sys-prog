include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = ancestors.c changecmdline.c changegroup.c displayvm.c  \
          hello_world.c memlayout.c print_elfphdr.c printelf.c showbreak.c \
          showbreakdiff.c spl_ps.c

OBJS    = $(patsubst %.c,%.o,$(SRCS)) elf_utils.o
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
	
elf_utils.o      : elf_utils.c     $(SPL_LIB) $(SPL_HDRS)
ancestors.o      : ancestors.c     $(SPL_LIB) $(SPL_HDRS)
changecmdline.o  : changecmdline.c  $(SPL_LIB) $(SPL_HDRS)
changegroup.o    : changegroup.c  $(SPL_LIB) $(SPL_HDRS)
displayvm.o      : displayvm.c    $(SPL_LIB) $(SPL_HDRS)
hello_world.o    : hello_world.c  $(SPL_LIB) $(SPL_HDRS)
memlayout.o      : memlayout.c    $(SPL_LIB) $(SPL_HDRS)
print_elfphdr.o  : print_elfphdr.c elf_utils.o  $(SPL_LIB) $(SPL_HDRS)
printelf.o       : printelf.c      elf_utils.o $(SPL_LIB) $(SPL_HDRS)
showbreak.o      : showbreak.c     $(SPL_LIB) $(SPL_HDRS)
showbreakdiff.o  : showbreakdiff.c  $(SPL_LIB) $(SPL_HDRS)
spl_ps.o         : spl_ps.c  ps_utils.o ps_utils.h $(SPL_LIB) $(SPL_HDRS)

