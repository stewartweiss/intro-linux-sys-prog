


SUBDIRS = $(wildcard chapter*) ipc

all:
	for dir in $(SUBDIRS); \
	do \
		$(MAKE) -C $$dir; \
  done

clean:
	for dir in $(SUBDIRS); \
	do \
	  $(MAKE) -C $$dir clean; \
	done

cleanall:
	for dir in $(SUBDIRS); \
	do \
	  $(MAKE) -C $$dir cleanall; \
	done

