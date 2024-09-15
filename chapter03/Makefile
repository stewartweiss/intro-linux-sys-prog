include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS   = spl_date1.c  spl_date2.c spl_date3.c spl_date4.c  \
       nl_langinfo_demo1.c strcoll_demo.c strfmon_demo.c newlocale_demo.c  \
       nl_langinfo_demo2.c 
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
	-rm -f $(OBJS)


spl_date1.o : spl_date1.c $(SPL_LIB) $(SPL_HDRS)
spl_date2.o : spl_date2.c $(SPL_LIB) $(SPL_HDRS)
spl_date3.o : spl_date3.c $(SPL_LIB) $(SPL_HDRS)
spl_date4.o : spl_date4.c $(SPL_LIB) $(SPL_HDRS)
nl_langinfo_demo1.o : nl_langinfo_demo1.c $(SPL_LIB) $(SPL_HDRS)
nl_langinfo_demo2.o : nl_langinfo_demo2.c $(SPL_LIB) $(SPL_HDRS)
strcoll_demo.o : strcoll_demo.c $(SPL_LIB) $(SPL_HDRS)
newlocale_demo.o : newlocale_demo.c $(SPL_LIB) $(SPL_HDRS)
strfmon_demo.o : strfmon_demo.c $(SPL_LIB) $(SPL_HDRS)
