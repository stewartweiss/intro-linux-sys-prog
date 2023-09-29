#!/bin/bash
# This script displays all of its command-line arguments in order.
# Its purpose is to demonstrate the use of the two shell parameters
# $0 and $#.
# Usage:  this_script  <word> <word> ...
#
# Written by Stewart Weiss, March, 2023

echo "The command that you typed is $0."
echo "The remaining words after the command are $*." 
