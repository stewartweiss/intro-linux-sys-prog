# Using Automatic Variables
# Written by Stewart Weiss
# Feb. 4, 2012
# Modified March 25, 2017 by Stewart Weiss
# 
# Make has a special feature called automatic variables. The values of automatic 
# variables are computed again for each rule that make executes, based on the 
# target and prerequisites of the rule. They are mostly used when you make your
# own "pattern rules" like the ones make uses as implicit rules, but they can
# also be useful in your ordinary rules.
# 
# Automatic variables only have values within the recipe of a rule, not within
# the target list or its prerequisites. They will expand to the empty string if
# used outside the recipe.
# 
# This makefile shows a few handy automatic variables and ways to use them.
# 
#   $@  is the full name of the current target. 
#   $^  is the list of all prerequisites of the target, with spaces between them
#   $?  is the list of prerequisites for the current target that are newer than
#       the target, with spaces between them.
#   $<  is the name of the first prerequisite. If the target got its recipe 
#       from an implicit rule, this is the first prerequisite added by the 
#       implicit rule. 

INSTALLDIR := ../bin
CC         := /usr/bin/gcc
CFLAGS     += -W -g
OBJECTS    := main.o utils.o interface.o

# This rule says to name the executable whatever the name of the target is,
# and to compile using all of the prerequisites. The second part of the recipe
# prints a list of the object files that forced lesson to be rebuilt. The "@"
# preceding the echo command tells make to execute the command without
# echoing it, which is what make does by default.
lesson: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@ echo Changed files: $?

main.o:  utils.h interface.h
utils.o: utils.h defs.h
interface.o: interface.h defs.h

.PHONY: clean 
clean:
	$(RM) $(OBJECTS) 

# The following rule exists only to demonstrate how the three automatic
# variables, $@, $^, and $?, work.  The target, dummy, is not a PHONY target,
# since we want to keep track of whether its prerequisites are newer than the
# target and will use a trick to do this. We create a dummy file, named dummy,
# that contains nothing. But when we run the rule, we touch the file so that
# its time of last modification is when we last ran the rule. This way the
# prerequisites will be older unless they were changed after we ran the rule.
dummy: $(OBJECTS)
	@ echo The prerequisites of dummy are $^
	@ echo The changed prerequisites are $?
	@ touch dummy


install: lesson
	@ if [ ! -d $(INSTALLDIR) ] ; then mkdir $(INSTALLDIR) ; fi
	\cp lesson $(INSTALLDIR)
	@ touch install

# This rule also uses the $? variable. It prints only those source files
# that have changed since the last time anything was printed. It uses the lpr
# command to send plain text to the default printer. It uses the same trick as
# we used for the dummy target above.
print: *.c *.h
	lpr -p $? 
	@ touch print

