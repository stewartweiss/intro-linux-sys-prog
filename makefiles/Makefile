# Recursive Makefiles
# written by Stewart Weiss
# September 10, 2007, revised Feb. 5, 2012
# 
# make can be invoked recursively. This is useful if you have a set of 
# subdirectories with their own makefiles and they are all part of a single
# system. You can write a single top-level makefile that causes make to read and
# apply the makefiles in each subdirectory.
# 
# This makefile does exactly that. It demonstrates how to use make recursively.
# It also shows an alternate way of writing shell commands using multiple
# lines for the commands.
# 
# To run the make command within a make file, never use the bare word "make".
# Instead use make's built-in MAKE variable, which always contains the path to 
# the version of make running the current command.
# 
# The -C option to make causes make to change directory to the argument to the
# -C option:
#     make -C foodir
# causes make to do a cd to foodir and then run the makefile inside foodir.
# 
# Here we also use the -k option to make, which tells it to press on even if 
# certain targets cause fatal errors and cannot be built; make just skips to 
# the next target.
# 
# This file begins by defining a recursively expanded variable named SUBDIRS 
# to contain all of the lesson subdirectories in the current directory. 
# In the recipe for the "all" target, this SUBDIRS variable is expanded to the
# list of directories whose name starts with "lesson".
# 
# The recipe for the "all" target is a shell command. In fact, all recipes 
# are passed by make to the shell to be executed, after stripping off the 
# leading tab character from each line. 
# In the makefiles in the lesson directories, the shell command was always written
# on a single line, as in
#
#    if [ ! -d $(INSTALLDIR) ] ; then mkdir $(INSTALLDIR) ; fi
#
# Each line in a recipe that ends in a newline character is treated as a 
# separate recipe to pass to the shell. But make lets you break a single recipe
# into multiple lines for readability. We could write the above recipe like this
#
#   if [  ! -d $(INSTALLDIR) ] ; \
#   then \
#       mkdir $(INSTALLDIR) ; \
#   fi
#
# where there is a NEWLINE immediately after the \ character, and a TAB at the
# start of each line. make will pass this command to the shell as 4 lines with 
# the embedded \-NEWLINE pairs. Most shells throw away the newline character 
# when this happens, treating the command as if it were written on one line.
# But if you use some shell that does not, the make file will not work.
#
# Next,  if you want a dollar sign to appear in your recipe, you must double it
# as in ‘$$’. In a shell that use dollar signs to introduce variables (such as 
# bash, the csh, the Bourne shell, etc. ) the $$ is necessary.
# For example, in the rule
#
#    LIST = a b c
#    all:
#        for i in $(LIST); do \
#            echo $$i; \
#        done
#
# LIST will be replaced by "a b c" and the variable name $$i will be replaced
# by $i and the shell will get the command
#     for i in a b c; do \
#         echo $i; \
#     done
#
# Now you are ready to understand the rules that follow.

SUBDIRS = $(wildcard lesson*) 

.PHONY: all
all:
	for dir in $(SUBDIRS); \
	do \
	    $(MAKE) -C $$dir -k; \
	done

.PHONY: clean
clean:
	for dir in $(SUBDIRS); \
	do \
	    $(MAKE) -C $$dir -k clean; \
	    if [ $$? -eq 2 ] ; then \rm $$dir/*.o ; fi \
	done

# The if command in here is needed because some of the makefiles do not have
# a clean target. To remove their .o files, I first check the exit status of
# make, which is 2 if there were errors, and if so, delete them with the shell
# command.
