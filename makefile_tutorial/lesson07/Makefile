# Using Implicit Variables
# Written by Stewart Weiss
# Feb. 4, 2012
# Revised March 20, 2017
#
# 
# The recipes in make's built-in implicit rules use certain predefined variables. 
# You can modify the values of these variables in the makefile. (You can also
# modify them when you run the make command.) Modifying these variables causes
# the implicit rules to use the values you give to these IMPLICIT VARIABLES. 
# 
# As an example, the beginning of the recipe in the implicit rule used to 
# compile a C source file is actually
# 
#     $(CC) -c $(CPPFLAGS) $(CFLAGS) 
# 
# The implicit variables used here are CC, CFLAGS, and CPPFLAGS. The default 
# value of CC is "cc" and the default values of CFLAGS and CPPFLAGS are empty 
# strings. Therefore, the implicit rule is actually
# 
#     cc -c 
# 
# If you modify the value of CC and CFLAGS say by writing
# 
#     CC      := /usr/bin/gcc
#     CFLAGS  := -Wall -g
# 
# then the implicit rule effectly becomes
# 
#     /usr/bin/gcc -Wall -g -c 
# 
# Note that all implicit rules that do C compilation use ‘$(CC)’ to get the 
# program name for the compiler and all of them include ‘$(CFLAGS)’ among the 
# arguments given to the compiler.
# 
# Some of the useful implicit variables to remember are listed below. The list
# of these can be found in the GNU Make Manual under the section 
# "Variables Used by Implicit Rules".
# 
# CC        Program for compiling C programs; default ‘cc’.
# CXX       Program for compiling C++ programs; default ‘g++’.
# CPP       Program for running the C preprocessor, with results to standard 
#               output; default ‘$(CC) -E’. 
# CFLAGS    Extra flags to give to the C compiler.
# CXXFLAGS  Extra flags to give to the C++ compiler. 
# LDFLAGS   Extra flags to give to compilers when they are supposed to invoke 
#               the linker, ‘ld’. 
# RM        Command to remove a file; default ‘rm -f’. (-f means "force" )

# Some of these variables are defined in your ENVIRONMENT, whereas others are
# defined in make's database.
# If you want to see the values assigned to these IMPLICIT VARIABLES on your 
# actual system in make's database, type the command:
# 
#     make -p -f/dev/null | more
#
# and scroll through the output. Or use grep, as in
#
#     make -p -f/dev/null | grep CPP
# 
# The remainder of this makefile takes advantage of these implicit variables
# and also introduces the concept of appending to variables.
#
# The next two lines are from the old makefile. They remain the same.
INSTALLDIR := ../bin
OBJECTS    := main.o utils.o interface.o

# This next line redefines the CC implicit variable to use the gcc compiler.
CC         :=    /usr/bin/gcc

# The next line redefines the CFLAGS implicit variable by APPENDING the text
# "-Wall -g" to it. The operator += appends text to a variable. There are other 
# ways to append text, but they will not work correctly if you use them with
# implicit variables such as CFLAGS. See the GNU Make Manual for an 
# explanation, or my explanation in the makefile in the lesson10 directory.
CFLAGS     += -Wall -g




lesson: $(OBJECTS)
	$(CC) $(CFLAGS) -o lesson $(OBJECTS)

main.o:  utils.h interface.h
utils.o: utils.h defs.h
interface.o: interface.h defs.h

# We can use the built-in RM variable now.
.PHONY: clean 
clean:
	$(RM) $(OBJECTS)

install: lesson
	@ if [ ! -d $(INSTALLDIR) ] ; then mkdir $(INSTALLDIR) ; fi
	\cp lesson $(INSTALLDIR)
	@ touch install


