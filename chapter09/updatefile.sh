#! /bin/bash
#  Repeatedly appends a character to the end of a file.
#  You can use this script to update files watched by watchfiles
#  Run it either in a second terminal window or in the background
#  before starting up watchfiles.

if [ $# -lt 1 ] ; then
    echo "usage: $0  file"
    exit
fi

for (( ; ; )) ; do
    echo -n "a" >> $1
    sleep 1 
done
    
