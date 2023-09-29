#!/bin/bash
# A script to add the sequence of numbers entered, one per line, on 
# standard input.
# usage:  sum.sh
#         It then waits for input. Type Ctrl-D to terminate input.
#
# Written by Stewart Weiss, May 2023
let done=1
let sum=0
while read n
do
    if [ $n -gt 0 ] ; then
        let sum=$sum+$n
    else
        let done=0
    fi
done
printf "%s\n" $sum 
    
