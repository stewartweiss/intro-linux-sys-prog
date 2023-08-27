#!/bin/bash
# ___This script demonstrates the use of positional parameters.___
echo "The command that you typed is $0."
echo -n "The first word after the command is '$1' "
echo "and the number of words is $#"
echo "The entire set of words is:"
echo $*
