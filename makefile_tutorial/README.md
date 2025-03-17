# ABOUT THESE DEMOS

This is a self-contained tutorial on using the `make` utility in Unix.
It is a set of directories designed to teach you how to write and use makefiles.
The directories are named `lesson1`, `lesson2`, ..., `lesson10`.
Each directory has a makefile in it. 
You should start with the makefile in directory `lesson1`, and
proceed in numerically increasing order through the directories. 
If you skip one, you will most likely not understand the rest, 
because each builds on the lessons from the preceding directories.

**Note: To use this tutorial, you should first go to the repository on GitHub 
that contains it, either by clicking on the "View on GitHub" button, 
or by manually loading the page 
[https://github.com/stewartweiss/Make-Tutorial](https://github.com/stewartweiss/Make-Tutorial). 
Once you have that page open in your browser, you should download the 
repository as a zipfile and extract it into a
directory on your Unix/Linux system. The rest of the instructions below 
assume you have done this. 
To download the repository as a zipfile, click the "`Clone or download`" 
button, and then click the "`Download ZIP`" link and 
follow the remaining instructions.**

The lessons do not cover all you need to know about makefiles.
They are designed to give you the basics, and perhaps a bit more.
Once you master these, you should read the *Make Manual*.

These makefiles are based on the GNU make command. GNU's version of make 
is not the same as other versions of make found in UNIX systems, but it
is POSIX-compliant. It has features not present in others, and it lacks some
features that are present in others. But since it is POSIX-compliant and
it is the most common version of make found on Linux systems, I chose to use 
it for teaching about make.

Each makefile has a lesson embedded in its comments. A comment in make
starts with the pound sign "#" and ends with a newline character.


# NAMING MAKEFILES

These lessons do not discuss how to name makefiles. I will briefly go over
that here. Makefiles are traditionally named either "`makefile`" or "`Makefile`".
The `make` command will look for a file with either name in the current working
directory. The GNU version of `make` tries the following names, in order: 

       GNUmakefile
       makefile
       Makefile  
You should not name your makefile `GNUmakefile`; it is misleading and not portable.
I prefer the uppercase version, i.e., `Makefile`, since all of my source 
files are always lowercase names and the other informational files, 
such as this `README`, are uppercase.
On many systems, the default collating sequence used by the `ls` command
is to display uppercase before lowercase, so the makefile name will be 
near the beginning of the directory listing. 


# RUNNING MAKE

To run the make command, make sure your working directory has a makefile 
in it. If it is not one of the standard names described above you can use the 
`-f` option to the make command. If the makefile is named `MAKEFILE`, you would type

    make -f MAKEFILE

to use that file instead. If you do not want `make` to do anything but 
just want to see what `make` might do, use the `-n` option, which does 
not execute any commands but just displays which ones it would do:

    make -n

There are many other things to know about running `make`, but these two options
are useful to remember as you progress through the lessons.

Lastly, the makefile in this top-level directory is the *LAST* part of this 
tutorial. You will not understand it until you have finished the tutorial.
*Save it for last*. 

Stewart Weiss
Sept. 5, 2023
