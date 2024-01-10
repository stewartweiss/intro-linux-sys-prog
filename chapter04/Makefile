# Type  make          to compile all the programs in the directory
#       make clean    to remove objects files and executables
#       make progname to make just progname

include ../Makefile.inc

CC      = /usr/bin/gcc
OBJS    = *.o
EXECS   = spl_date1  spl_date2 spl_date3 spl_date4 \
          nl_langinfo_demo strcoll_demo strfmon_demo newlocale_demo
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

spl_date1:  spl_date1.o
	$(CC) $(CFLAGS) spl_date1.o  $(LDFLAGS) -o $@

spl_date2:  spl_date2.o
	$(CC) $(CFLAGS) spl_date2.o  $(LDFLAGS) -o $@

spl_date3:  spl_date3.o
	$(CC) $(CFLAGS) spl_date3.o  $(LDFLAGS) -o $@

spl_date4:  spl_date4.o
	$(CC) $(CFLAGS) spl_date4.o  $(LDFLAGS) -o $@

nl_langinfo_demo:  nl_langinfo_demo.o
	$(CC) $(CFLAGS) nl_langinfo_demo.o  $(LDFLAGS) -o $@

strcoll_demo:  strcoll_demo.o
	$(CC) $(CFLAGS) strcoll_demo.o  $(LDFLAGS) -o $@

newlocale_demo:  newlocale_demo.o
	$(CC) $(CFLAGS) newlocale_demo.o  $(LDFLAGS) -o $@

strfmon_demo:  strfmon_demo.o
	$(CC) $(CFLAGS) strfmon_demo.o  $(LDFLAGS) -o $@
