# Type  make          to compile all the programs in the directory
#       make clean    to remove objects files and executables
#       make progname to make just progname

include ../Makefile.inc

CC      = /usr/bin/gcc
OBJS    = *.o
EXECS   = showdate1  showdate2 showdate3 showdate4 \
          langinfo_demo smallest monetary_demo newlocale_demo
CFLAGS  += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  \
           -Wall -g -I${SPL_INCLUDE_DIR}
LDFLAGS += -L ${SPL_LIB_DIR} -lspl

.PHONY: all clean cleanall

all: $(EXECS)

clean:
	-rm -f $(OBJS)

cleanall:
	-rm -f $(OBJS) $(EXECS)

.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@

showdate1:  showdate1.o
	$(CC) $(CFLAGS) showdate1.o  $(LDFLAGS) -o $@

showdate2:  showdate2.o
	$(CC) $(CFLAGS) showdate2.o  $(LDFLAGS) -o $@

showdate3:  showdate3.o
	$(CC) $(CFLAGS) showdate3.o  $(LDFLAGS) -o $@

showdate4:  showdate4.o
	$(CC) $(CFLAGS) showdate4.o  $(LDFLAGS) -o $@

langinfo_demo:  langinfo_demo.o
	$(CC) $(CFLAGS) langinfo_demo.o  $(LDFLAGS) -o $@

smallest:  smallest.o
	$(CC) $(CFLAGS) smallest.o  $(LDFLAGS) -o $@

newlocale_demo:  newlocale_demo.o
	$(CC) $(CFLAGS) newlocale_demo.o  $(LDFLAGS) -o $@

monetary_demo:  monetary_demo.o
	$(CC) $(CFLAGS) monetary_demo.o  $(LDFLAGS) -o $@
