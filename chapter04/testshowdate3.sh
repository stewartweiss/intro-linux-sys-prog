#!/bin/bash

echo showdate3 -h ; showdate3 -h
echo showdate3 +"%a %b %d, %Y, at %R" ; showdate3 +"%a %b %d, %Y, at %R"
echo showdate3 -d '1 year'  +"%a %b %d, %Y, at %R" ; showdate3 -d '1 year'  +"%a %b %d, %Y, at %R"
echo showdate3 -d '1 week 2 hours'  +"%a %b %d, %Y, at %R" ; showdate3 -d '1 week 2 hours'  +"%a %b %d, %Y, at %R"
echo showdate3 -d '-2 months   +4 months'  +"%a %b %d, %Y, at %R" ; showdate3 -d '-2 months   +4 months'  +"%a %b %d, %Y, at %R"
echo showdate3 -d ' +120 minutes -2 hours '  +"%a %b %d, %Y, at %R" ; showdate3 -d ' +120 minutes -2 hours '  +"%a %b %d, %Y, at %R"
