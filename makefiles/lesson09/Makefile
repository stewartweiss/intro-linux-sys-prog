# Wildcards, the Wildcard Function, and Pattern Functions
# Written by Stewart Weiss
# Feb. 4, 2012
#
# This makefile introduces wildcards, the wildcard function, and the pattern
# substitution function. 
# 
# You have seen in previous example makefiles that we can use shell wildcards
# in prerequisite lists and in recipes, as in
#     clean:
#         \rm *.o
# and
#     print: *.c
#         lpr -p $?
# 
# We can also use wildcards in target lists, although we will not do that here.
# Using them in variable definitions is useful but requires some care. 
# We begin with the type of variable defined using the ":=" operator. The only
# way to assign a wildcard-like expression to a variable of this kind is to use
# the wildcard operator. This operator is of the form
#     $(wildcard  pattern )
# where pattern contains some wildcard such as *, ?, or [].
# For example:
#     SRCS       := $(wildcard *.c )
# makes the variable SRCS contain a list of all .c files in the current 
# directory, and 
#     HDRS        := $(wildcard *.h )
# would make HDRS contains a list of header files.
# 
# If you want to create a variable containing the list of object files that 
# would be produced by compiling all of the .c files, then it is not enough to
# write
#     OBJS        := $(wildcard *.o )
# because there may not be any object files in the directory when this is 
# expanded, and so OBJS would contain nothing. You would get an error if you
# wrote 
#     lesson: $(OBJS)
#	         gcc -o lesson $(OBJS)
#
# The solution is to use the pattern substitution operator, $(patsubst ...).
# This operator has the form 
#    $(patsubst pattern,replacement,text)
# In pattern, % is a wildcard matching anything, and if the replacement also 
# contains a ‘%’, the ‘%’ is replaced by the text that matched the ‘%’ in 
# pattern.  
# For example:
#
#    $(patsubst %.c, %.o, bar.c)
#
# produces the string "bar.o" because the % in %.c "stores" the string "bar" 
# and in %.o, it is replaced by "bar".
# If SRCS = "main.c utils.c interface.c"
#
#    $(patsubst %.c, %.o, $(SRCS) )
#
# produces "main.o utils.o interface.o". Notice that each word in SRCS is 
# used by the patsubst operator independently, as if it were written three times.
# So this is one way to get a list of required object files. 
# (There are other ways.)

# Our makefile follows:

CC         := /usr/bin/gcc
CFLAGS     += -W -g
INSTALLDIR := ../bin
SRCS       := $(wildcard *.c )
OBJS       := $(patsubst %.c, %.o, $(SRCS))
EXEC       := lesson

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS)  -o $@  $(OBJS)

main.o:  utils.h interface.h
utils.o: utils.h defs.h
interface.o: interface.h defs.h

# This adds a new use for a PHONY target. Sometimes we want to remove
# just the object files, and other times we want to remove the executable
# as well. By making cleanall a PHONY target that depends on clean, we
# force clean to be updated, meaning run, when we make the target cleanall
# or we can just run make clean to remove the object files alone.
.PHONY: clean cleanall

cleanall: clean
	$(RM) $(EXEC)

clean:
	$(RM) $(OBJS)

install: lesson
	@ if [ ! -d $(INSTALLDIR) ] ; then mkdir $(INSTALLDIR) ; fi
	\cp lesson $(INSTALLDIR)
	@ touch install
	
