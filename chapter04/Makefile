# Type  make          to compile all the programs in the directory
#       make clean    to remove objects files and executables
#       make progname to make just progname
# This relies on the implicit rule
#    $(CC) $(LDFLAGS) x.o $(LOADLIBES) $(LDLIBS)
# to link the files into x, and  the rule
#    $(CC) -c $(CPPFLAGS) $(CFLAGS)  x.c
# to make x.o from x.c


include ../Makefile.inc

CC        = /usr/bin/gcc
OBJS      = *.o
EXECS     = spl_date1  spl_date2 spl_date3 spl_date4 \
          nl_langinfo_demo strcoll_demo strfmon_demo newlocale_demo \
          nl_langinfo_demo2
CFLAGS   += -D_XOPEN_SOURCE=700  -D_DEFAULT_SOURCE  -Wall -g
CPPFLAGS += -I${SPL_INCLUDE_DIR}
LDFLAGS  += -L ${SPL_LIB_DIR}
LDLIBS   +=  -lspl
VPATH     = ../include

.PHONY: all clean cleanall

all: $(EXECS)


cleanall: clean
	-rm -f $(EXECS)

clean:
	-rm -f $(OBJS)


spl_date1.o:     common_hdrs.h get_nums.h error_exits.h
spl_date2.o:     common_hdrs.h get_nums.h error_exits.h
spl_date3.o:     common_hdrs.h get_nums.h error_exits.h
spl_date4.o:     common_hdrs.h get_nums.h error_exits.h
nl_langinfo_demo.o: common_hdrs.h get_nums.h error_exits.h
nl_langinfo_demo2.o: common_hdrs.h get_nums.h error_exits.h
strcoll_demo.o:  common_hdrs.h get_nums.h error_exits.h
newlocale_demo.o: common_hdrs.h get_nums.h error_exits.h
strfmon_demo.o:  common_hdrs.h get_nums.h error_exits.h
