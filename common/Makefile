# Makefile for common code
# Type  make          to build the library and header file
#       make clean    to remove objects files and executables
#       make install  to copy the library into ../lib and the
#                     header to ../include

include ../Makefile.inc

INSTALLDIR = ../lib
INCLUDEDIR = ../include
CC         = /usr/bin/gcc
AR         = /usr/bin/ar
SRCS       = $(wildcard *.c)
OBJS       = $(patsubst %.c,%.o,$(SRCS))
CFLAGS    += -Wall -g

.PHONY: all clean cleanall install
all: libspl.a

install: libspl.a
	cp libspl.a $(INSTALLDIR)
	cp *.h $(INCLUDEDIR)


.c.o:
	$(CC) $(CFLAGS) -c -fPIC  $<

clean:
	\rm $(OBJS)

libspl.a: $(OBJS)
	$(AR) rcs libspl.a  $(OBJS)


