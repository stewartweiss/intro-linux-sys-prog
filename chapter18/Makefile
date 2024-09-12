include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = fakeinput_demo.c fakelogin.c fakelogin_ioctl.c fillqueue.c \
          max_input.c setecho.c spl_stty.c toggle_echo.c upcopy_fread.c \
          upcopychars.c watchtty.c winsizechange_demo.c
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

# All executables should be dependent on the common directory files
fakeinput_demo.o:  fakeinput_demo.c   $(SPL_LIB) $(SPL_HDRS)
fakelogin.o:       fakelogin.c   $(SPL_LIB) $(SPL_HDRS)
fakelogin_ioctl.o: fakelogin_ioctl.c   $(SPL_LIB) $(SPL_HDRS)
fillqueue.o:       fillqueue.c   $(SPL_LIB) $(SPL_HDRS)
max_input.o:       max_input.c   $(SPL_LIB) $(SPL_HDRS)
spl_stty.o:        spl_stty.c   $(SPL_LIB) $(SPL_HDRS)
toggle_echo.o:     toggle_echo.c   $(SPL_LIB) $(SPL_HDRS)
upcopy_fread.o:    upcopy_fread.c   $(SPL_LIB) $(SPL_HDRS)
upcopychars.o:     upcopychars.c   $(SPL_LIB) $(SPL_HDRS)
watchtty.o:        watchtty.c   $(SPL_LIB) $(SPL_HDRS)
winsizechange_demo.o: winsizechange_demo.c   $(SPL_LIB) $(SPL_HDRS)
