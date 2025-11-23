# System Programming in Linux: A Hands-On Introduction "Demo" Programs

This repository contains source code for the programs in the first printing
of my book, "System Programming in Linux: A Hands-On Introduction".  The
book is published by No Starch Press and available on Amazon here:
https://www.amazon.com/System-Programming-Linux-Stewart-Weiss/dp/1718503563.
You can read more about it on the webpage
https://nostarch.com/introduction-system-programming-linux.

## Building the Programs
For instructions on building the programs, see the section
`How To Use This Repository'.

As more and more people read the book and use this code, 
they will find bugs in the code that I missed, and/or alternative 
code that might run faster or be better in some other way than what
you see here. To see the modified code, you should checkout the "main"
branch on the GitHub repository.
To see a summary of changes that have been made to the code since it was
published in the book, see the CHANGES file in the main branch..


## Licensing

All complete programs provided in this repository are covered by
the GNU General Public License (Version 3), a copy of which is
contained in the file COPYING.gplv3 in this directory.
The source code for all library functions (in the common/ and include/
directories) is
covered by the GNU Lesser General Public License (Version 3), a copy of
which is in the file COPYING.lgplv3 in this directory.

## Organization of Subdirectories
The subdirectories are either named by chapter, in the form Chapter*NN*,
or have names such as "include", "lib", "makefiles", and so forth.
The Chapter*NN* directories contain code introduced in the corresponding
chapter of the book.
The other chapters are self-explanatory.



I welcome suggestions, corrections, discovery of bugs, and other improvements.
At present there is no CONTRIBUTING file because the instructions are fairly simple ---
If you see something that needs improvement,
create an issue with as much detail as possible.
Please ensure your description is clear and has sufficient instructions to be able to reproduce the issue.

## How To Use This Repository

Each chapter is a self-contained collection of programs. If a chapter has
a `README` file, you should read that file before doing anything in that chapter.

All program code depends on the files in the `common` directory. To build
the programs in any chapter, you must set up your environment as follows:

1. After cloning the repository, `cd` into the common directory and run `make`:

   ```bash
      $ cd common
      $ make
   ```

1. Then run `make install` in that directory:

   ```bash
      $ make install
   ```
   This copies the header file created by `make` into the `include` directory in
   this repository, and the static library `libutils.a` into the `lib`
   directory.

1. Now `cd` into the chapter you'd like to build and run `make` there, e.g.

   ```bash
     $ cd ../chapter05
     $ make
   ```

