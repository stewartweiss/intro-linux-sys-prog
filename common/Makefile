include ../Makefile.inc

INSTALLDIR = ../lib
INCLUDEDIR = ../include
CC         = /usr/bin/gcc
AR         = /usr/bin/ar
SRCS       = $(wildcard *.c)
HDRS       = $(wildcard *.h)
OBJS       = $(patsubst %.c,%.o,$(SRCS))
CFLAGS    += -Wall -g

.PHONY: all clean cleanall install
all: libspl.a 

install: libspl.a $(HDRS)
	cp libspl.a $(INSTALLDIR)
	cp $(HDRS) $(INCLUDEDIR)


.c.o:
	$(CC) $(CFLAGS) -c -fPIC  $<

clean:
	\rm $(OBJS) 

cleanall: clean 
	$(RM) libspl.a

libspl.a: $(OBJS)  $(HDRS)
	$(AR) rcs libspl.a  $(OBJS)
	cp libspl.a $(INSTALLDIR)


