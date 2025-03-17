# Using Implicit Rules
# Written by Stewart Weiss
# September 10, 2007
# Revised Feb. 4, 2012
# Revised March 20, 2017
#
# This makefile is the same as the one in directory lesson01 except that there are 
# no recipes for main.o, utils.o, or interface.o.
#
# This is because make has implicit rules that it uses to build certain files. 
# 
# For example, make "knows how" to make an object file X.o from a
# source file X.c -- it runs the C compiler on it. Similarly, 
# X.o is made automatically from X.cc or X.C by running the C++ compiler on it.
#
# make has no implicit rule  to build the lesson program of our example obviously,
# because it does not know what we would want to name our executable. 
# If it were not for that, it could build lesson knowing only its prerequisites. 
# The list of implicit rules is contained in the GNU Make Manual, under the
# heading "Catalogue of Implicit Rules". (But do not look at it until you have
# finished reading all of these lesson Makefiles, because you will not understand
# them yet!)
#
# A makefile becomes simpler when it takes advantage of implicit rules.
# This makefile is smaller and simpler than its predecessor because of this.
#
# In this makefile, the three targets named main.o, utils.o, and interface.o
# are built using the implicit rule that an object file X.o can be built from
# a C source file by running the C compiler on it and passing appropriate flags,
# which we will talk about shortly.

lesson: main.o utils.o interface.o
	gcc -o lesson main.o utils.o interface.o

main.o: main.c utils.h interface.h  
utils.o: utils.c utils.h defs.h
interface.o: interface.c interface.h defs.h

# Notice that we do not need blank lines between rules, but sometimes they
# make the file easier to read.
