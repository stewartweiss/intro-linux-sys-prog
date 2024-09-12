include ../Makefile.inc

CC      = /usr/bin/gcc
SRCS    = shm_creator_demo1.c shm_user_demo1.c shm_creator_demo2.c \
          shm_user_demo2.c shm_consumer.c shm_producer.c \
          unnamedsem_demo.c namedsem_demo.c \
          mqrcv_demo.c mqsend_demo.c \
          ulogger.c ulogger_client.c 

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


shm_consumer.o:      shm_consumer.c shm_prodcons.h $(SPL_LIB) $(SPL_HDRS)
shm_producer.o:      shm_producer.c shm_prodcons.h $(SPL_LIB) $(SPL_HDRS)
shm_creator_demo1.o: shm_creator_demo1.c shm_demo1.h $(SPL_LIB) $(SPL_HDRS)
shm_user_demo1.o:    shm_user_demo1.c shm_demo1.h  $(SPL_LIB) $(SPL_HDRS)
shm_creator_demo2.o: shm_creator_demo2.c shm_demo2.h $(SPL_LIB) $(SPL_HDRS)
shm_user_demo2.o:    shm_user_demo2.c shm_demo2.h  $(SPL_LIB) $(SPL_HDRS)
unnamedsem_demo.o:   unnamedsem_demo.c             $(SPL_LIB) $(SPL_HDRS)
namedsem_demo.o:     namedsem_demo.c               $(SPL_LIB) $(SPL_HDRS)
mqrcv_demo1.o:       mqrcv_demo1.c                 $(SPL_LIB) $(SPL_HDRS)
mqsend_demo1.o:      mqsend_demo1.c                $(SPL_LIB) $(SPL_HDRS)
ulogger.o:           ulogger.c ulogger.h           $(SPL_LIB) $(SPL_HDRS)
ulogger_client.o:    ulogger_client.c ulogger.h    $(SPL_LIB) $(SPL_HDRS)
