


SUBDIRS = $(wildcard chapter*)

all:
	$(MAKE) -C common 
	$(MAKE) -C common install
	for dir in $(SUBDIRS); \
	do \
		$(MAKE) -C $$dir; \
  done

clean:
	$(MAKE) -C common clean
	for dir in $(SUBDIRS); \
	do \
	  $(MAKE) -C $$dir clean; \
	done

cleanall:
	$(MAKE) -C common cleanall
	for dir in $(SUBDIRS); \
	do \
	  $(MAKE) -C $$dir cleanall; \
	done

