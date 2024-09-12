#!/bin/bash

if [ $# -lt 1 ] ; then
    exit
fi
if [ -x $1 ] ; then
    readelf -h $1 | grep 'Class:' | awk '{print $2}'
fi
