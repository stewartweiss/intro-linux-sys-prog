# Using Implicit Prerequisites
# Written by Stewart Weiss
# September 10, 2007
# Revised Feb. 4, 2012
# Revised March 20, 2017
#
# The preceding makefile introduced the use of make's implicit rules to 
# simplify the makefile. It purposely avoided the use of implicit 
# prerequisites.
#
# In general, when a recipe is omitted from a rule because an implicit rule
# is being used, the implied prerequisite can also be omitted.
#
# For example, when an implicit rule is used to make a '.o' file from a '.c’ 
# file, it is also automatically added to the list of prerequisites. There
# is no need for you to put the '.c' file in the prereqisite list of a
# target that depends upon it. The makefile becomes even simpler, as this one
# demonstrates.

lesson: main.o utils.o interface.o
	gcc -o lesson main.o utils.o interface.o

main.o: utils.h interface.h  
utils.o: utils.h defs.h
interface.o: interface.h defs.h

# WARNING
# How does make know to use the C compiler to make utils.o?  
# An overly simplified  answer is that it looks in the directory for a 
# file whose basename is utils and then looks at the extension and applies
# the rule. If there had been a file named utils.F instead of utils.c, it would
# have used the Fortran compiler. If you have two source files, say one named
# utils.F and the other named utils.c, it will use the C compiler, because the
# rule for '.c' files precedes the rule for 'F' files in its list of rules.
# You should make sure you never have this situation. If you do have files that
# are named the same, you should create different directories, one for Fortran
# sources, for example, and another for C sources.
