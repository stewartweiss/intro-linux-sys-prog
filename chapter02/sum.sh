#!/bin/bash
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
    
