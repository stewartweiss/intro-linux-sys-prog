# Using Variables
# Written by Stewart Weiss
# September 10, 2007
# Revised Feb. 4, 2012
# Revised March 20, 2017
#
# Makefiles can have variables. Variables allow makefiles to be simpler and 
# less error-prone. A variable is a name defined in a makefile to represent 
# a string of text, called the variable's value. 
# 
# Variable names can be any sequence of letters, digits, and underscores.
# (They can actually have other characters but they should not for reasons I 
# will not explain here.)
# Variable names are case-sensitive: FOO and foo would be different variables.
# It is traditional to use upper case letters in variable names.
# 
# You define a variable with a line of the form
#     VAR = value
# or
#     VAR := value
# The difference will be explained in a later makefile example.
# White space around the variable name and immediately after the ‘=’ is ignored. 
#
# You use a variable by putting it in parentheses or braces, preceded by a 
# dollar sign:
#    $(VAR)  or ${VAR}
# 
# Variables used this way are replaced by their value as a text string.
# 
# The following lines define four variables. 

INSTALLDIR := ../bin
COMPILER   :=    /usr/bin/gcc
FLAGS      :=
OBJECTS    := main.o utils.o interface.o


# Although leading spaces are ignored, trailing spaces (up to the newline or 
# symbol are part of the value. Assuming no trailing spaces are above 
# (which they are not), the variable FLAGS is the empty string. The variable COMPILER 
# is the string "/usr/bin/gcc", and OBJECTS is the string 
# "main.o utils.o interface.o". 

# The rule for the lesson target is now simpler. In addition, if we decided to
# add another source file, we would only have to modify the definition of
# OBJECTS. The rule for the phony target uses the variable INSTALLDIR. If we
# want to change the install directory, we change just the variable value.

lesson: $(OBJECTS)
	$(COMPILER) $(FLAGS) -o lesson $(OBJECTS)

main.o:  utils.h interface.h
utils.o: utils.h defs.h
interface.o: interface.h defs.h

.PHONY: clean 
clean:
	\rm $(OBJECTS)

install: lesson
	@ if [ ! -d $(INSTALLDIR) ] ; then mkdir $(INSTALLDIR) ; fi
	\cp lesson $(INSTALLDIR)
	@ touch install

# What are those @ symbols doing on those lines? Are they typos? 
# NO:
# The above rule for target install adds a new feature. As you have probably 
# noticed by now, make is verbose by default, which means that it echoes every 
# command in each recipe that it runs. (That is what "verbose" means in UNIX.)
# This is not always what you want.  To turn off echoing of a specific command, 
# you can put the "@" character before the command, after the tab character. 
# I turned off the make command's verboseness on the two commands in the 
# install target's recipe.


