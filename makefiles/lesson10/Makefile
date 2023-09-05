# The Two Types of Variables
# Written by Stewart Weiss
# September 10, 2007, revised Feb. 4, 2012
#
# As shown earlier there are two different kinds of variables in makefiles. Each 
# kind has advantages over the other, but the safer one to use in your makefiles
# is the kind defined using the ":=" operator.
#
# This makefile demonstrates the difference between these two types of 
# variables. The ones defined using the "=" operator are called recursively
# expanded variables, and those defined using the ":=" operator are called
# simply expanded variables. 
#
# Simply expanded variables behave the way a variable in a typical modern
# programming language behaves. When you assign an expression to it, the 
# expression is evaluated immediately and its value is assigned to the variable.
#
# Recursively expanded variables behave in an unexpected way. The expression is
# not evaluated until the variable is actually used, i.e., when it is 
# substituted using the $() or ${} syntax. So if the variable's value contains
# other variables, then if these change value after the initial assignment, 
# their new values are used rather than the old ones.
#
# An example will illustrate.
# Following are definitions of x,y,w, and z. The x and y are simply expanded,
# and w and z, recursively expanded. The show1 target below them simply echos
# the values of y and z. Try to predict what they will output before typing
# make show1.
 

x := This is better than 
y := $(x) I thought.
x := This is worse than

w = This is better than 
z = $(w) I thought.
w = This is worse than

.PHONY: show1
show1:
	@ echo The value of y is: $(y)
	@ echo The value of z is: $(z)

# Because recursively expanded variables are not expanded until they are used,
# the following lines result in an infinite loop (except that make detects 
# this and halts it.)
#
#    OBJS  = main.o utils.o
#    OBJS  = $(OBJS)  interface.o
#
# This is because $(OBJS) is expanded using the second line and we will get 
# a value of the form interface.o interface.o interface.o ....
# Instead you must use
#
#    OBJS  = main.o utils.o
#    OBJS  := $(OBJS)  interface.o
# to convert OBJS to a simply expanded variable.
# This is why you cannot write "CFLAGS = $(CFLAGS) -W -g".  And if you write
#    
#    CFLAGS  := $(CFLAGS)  -W -g
#
# in a program, you convert CFLAGS to a simply expanded variable and it gets
# broken in the implicit rules, because CFLAGS is defined within make by
#    CFLAGS = $(includes) -O
# and the variable "includes" will not get expanded in the implicit rules where
# it must be.

# Here is a second example, just to make sure you follow this. 

text1    :=  This is text1
text2    :=  and this is text2
message1 :=  $(text1) $(text2)
message2  =  $(text1) $(text2)
text1     = $(string1) blah blah $(string2)
string1   = XXXXXX
string2   = YYYYYY

.PHONY: show2
show2:
	@ echo Message1: $(message1)
	@ echo Message2: $(message2)
	

