#!/bin/bash

if [ ! -x testspl_date3 ] ; then
    echo "Make sure the executable, spl_date3, exists before running this script."
    exit
fi

echo testspl_date3 -h ; testspl_date3 -h
echo testspl_date3 +"%a %b %d, %Y, at %R" ; testspl_date3 +"%a %b %d, %Y, at %R"
echo testspl_date3 -d '1 year'  +"%a %b %d, %Y, at %R" ; testspl_date3 -d '1 year'  +"%a %b %d, %Y, at %R"
echo testspl_date3 -d '1 week 2 hours'  +"%a %b %d, %Y, at %R" ; testspl_date3 -d '1 week 2 hours'  +"%a %b %d, %Y, at %R"
echo testspl_date3 -d '-2 months   +4 months'  +"%a %b %d, %Y, at %R" ; testspl_date3 -d '-2 months   +4 months'  +"%a %b %d, %Y, at %R"
echo testspl_date3 -d ' +120 minutes -2 hours '  +"%a %b %d, %Y, at %R" ; testspl_date3 -d ' +120 minutes -2 hours '  +"%a %b %d, %Y, at %R"
