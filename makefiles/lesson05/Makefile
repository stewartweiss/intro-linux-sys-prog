# Adding "PHONY" Targets
# Written by Stewart Weiss
# September 10, 2007
# Revised Feb. 4, 2012
# Revised March 20, 2017
#
# This makefile introduces "phony" targets, which are targets that are
# not the names of files, but are simply a means to execute sets of commands
# (i.e., recipes.)
# For example,
#
# clean:
#      rm *.o
#
# is a rule whose target is "clean", with no prerequisites. Its recipe is
# executed whenever "clean" must be "built". But its recipe does not create a
# file named "clean"; it deletes all object files in the directory. The command
#     rm *.o 
# is a shell command, and the "*.o" is expanded by the shell to include all .o
# files in the current directory.
# To run this command, you type
#
#    make clean
#
# If a file named "clean" actually existed in our directory, this rule would
# not work, because "clean" has no prerequisites and so it would always be 
# newer than its dependencies (since it has none) and so its recipe would
# never be run. If someone inadvertently put a file named clean in
# the directory, our makefile would be broken. 
#
# To avoid that, you can explicitly tell make that "clean" is a "phony" target.
# You do this with the line:
#
#    .PHONY: clean 
#
# This line uses the special target ".PHONY". It says that the word "clean" 
# is a phony target. so even if there is a file named clean in the directory,
# make will ignore it and run the rule for the phony target "clean".

# If you recall, in lesson02, we had a target named all whose purpose was to
# force make to update its list of prerequisites. That target was also phony
# so we could have declared it as a PHONY target as well.

.PHONY: all
all: lesson

lesson: main.o utils.o interface.o
	gcc -o lesson main.o utils.o interface.o

main.o: main.c  utils.h interface.h
utils.o: utils.c utils.h defs.h
interface.o: interface.c interface.h defs.h

# The rule to remove object files is:
.PHONY: clean
clean:
	\rm *.o

# Notice that I use "\rm *.o",not "rm *.o".  The reason for this is that the
# rm command might be aliased to be interactive, rm -i. Prefixing a command 
# with backslash turns off aliasing. I could also write /usr/bin/rm, but I 
# would have to be certain that this is the correct absolute path on the system.

# The following is not a phony target, but an empty target file that has the 
# same effect. To be able to type 
#     make install
# and have the lesson executable copied into the directory ../bin, we could use
# a phony target named install, but instead I use a file named install that 
# depends on lesson. If lesson does not exist, it is made by this command.
# The recipe also has a command to touch the file install. This changes its
# time of last modification to the time the last install was performed. This
# serves as a record for when the file was copied into our bin directory.
# If lesson has not been updated since the last install, this will not copy
# anything into ../bin.

# It is common to supply a "make install" command when providing software  
# packages for distribution. This is one way to do it.


install: lesson
	\cp lesson ../bin
	touch install

# If the bin directory did not exist, this would create a file named bin in the
# parent directory. This is not what we want. The following is error-proof.

install2: lesson
	if [ ! -d ../bin ] ; then mkdir ../bin ; fi
	\cp lesson ../bin
	touch install2

# This is another example of a phony target used to create a compressed tar
# file of this source code distribution. It will create a tar file containing
# the .c and .h files and the makefile, and name it lesson.tar. It does this by
# creating a temporary directory named lessondir, linking all files into it,
# and tarring this directory.
tar: *.c *.h
	if [ ! -d lessondir ] ; then mkdir lessondir ; fi
	ln -f Makefile *.c *.h lessondir/
	tar -czf lesson.tar.gz lessondir
	rm -rf lessondir

