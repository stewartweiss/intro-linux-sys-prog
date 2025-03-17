# Using Makefiles
# Written by Stewart Weiss
# September 10, 2007, 
# Revised Feb. 2, 2012
# Revised March 19, 2017
#
# This makefile is identical to the first one except that the first rule is new,
# and its purpose is to explain a bit about how you use the make command.
# When you type 
#     make
# by itself with no command-line arguments, make looks for the FIRST target in 
# the makefile and tries to build it.  In this makefile, the first target is 
# the word "all". The target "all" has no recipe, but it has dependencies. 
# make does not try to create a target named all. Instead it tries to update
# each of the dependencies and do nothing more. In this case, "all"
# depends on the file named lesson, and so typing "make" by itself will cause 
# lesson to be built or updated if any of its dependencies have changed.

# 
# You can also use the make command with arguments. Typing
#     make lesson
# causes make to look for a target named lesson and build it if it finds it.
# 
# Typing 
#     make main.o
# causes make to build main.o, and typing
#     make spaghetti
# causes make to display the error message
#     make: *** No rule to make target `spaghetti'.  Stop.
# because there is no target with that name. Too bad. 
#
# If you want to force make to rebuild a target, you can type the command
#     touch <prereq>
# where <prereq> is one of the target's prerequisites. For example,
#     touch defs.h
# will cause make to rebuild utils.o, interface.o, and lesson, since the first 
# two directly depend on defs.h, and lesson depends on them.
# 
# You will probably see makefiles in which the first line looks something like
# this:
#     all: prog1 prog2 prog3 prog4 prog5
# 
# Very often a directory will contain the sources for many programs and a single
# makefile can be used to build them. In this case, each program whose name 
# appears in the prerequisite list of the first target will be built. The target
# "all" is not predefined; it is just a common sense word to use for it.
#
# To illustrate this idea, this directory has the sources for two different 
# programs, named lesson and driver. They differ only in their main programs. In
# other words, each depends on the same files except for main.c and driver.c.
# The first line says that target "all" depends on both lesson and driver.
# The target "all" has no RECIPE because its only purpose is to force lesson and
# driver to be updated.
# The remaining lines are rules for building each of these.


all: lesson driver

lesson: main.o utils.o interface.o
	gcc -o lesson main.o utils.o interface.o

driver: driver.o utils.o interface.o
	gcc -o driver driver.o utils.o interface.o

main.o: main.c utils.h interface.h
	gcc -c main.c

driver.o: driver.c utils.h interface.h
	gcc -c driver.c

utils.o: utils.c utils.h defs.h
	gcc -c utils.c

interface.o: interface.c interface.h defs.h
	gcc -c interface.c

