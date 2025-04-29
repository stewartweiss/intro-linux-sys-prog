This repository contains source code for the programs in my book
"Introduction to System Programming in Linux".
The book will be published by No Starch Press and available on Amazon
on October 15, 2025.
You can read more about it on the webpage
https://nostarch.com/introduction-system-programming-linux.

Building the Programs
For instructions on building the programs, see the section
`How To Use This Repository'.

For notes on changes that have been made to the code since it was
published in the book, see the CHANGES file.

Licensing

All complete programs provided in this repository are covered by
the GNU General Public License (Version 3), a copy of which is
contained in the file COPYING.gplv3 in this directory.
The source code for all library functions (in the common/ and include/
directories) is
covered by the GNU Lesser General Public License (Version 3), a copy of
which is in the file COPYING.lgplv3 in this directory.

Organization of Subdirectories
The subdirectories are either named by chapter, in the form ChapterNN,
or have names such as "include", "lib", "makefiles", and so forth.
The ChapterNN directories contain code introduced in the corresponding
chapter of the book.
The other chapters are self-explanatory.

I welcome suggestions, corrections, discovery of bugs, and other improvements.
At present there is no CONTRIBUTING file because the instructions are fairly simple ---
If you see something that needs improvement,
create an issue with as much detail as possible.
Please ensure your description is clear and has sufficient instructions to be able to reproduce the issue.

How To Use This Repository

Each chapter is a self-contained collection of programs. If a chapter has
a `README` file, you should read that file before doing anything in that chapter.

All program code depends on the files in the  common  directory. To build
the programs in any chapter, you must set up your environment as follows:

1. After cloning the repository,  cd  into the common directory and run  make :

      $ cd common
      $ make


2. Then run  make install  in that directory:

      $ make install

   This copies the header file created by  make  into the  include  directory
   in this repository, and the static library  libspl.a  into the  lib
   directory.

3. Now  cd  into the directory you'd like to build and run  make  there, e.g.

     $ cd ../chapter05
     $ make


