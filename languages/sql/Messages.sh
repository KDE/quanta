#!/bin/sh
$EXTRACTRC `find . -name \*.rc` `find . -name \*.ui` >>rc.cpp
$XGETTEXT `find . -name \*.cc -o -name \*.cpp -o -name \*.h` -o $podir/kdevsql.pot
rm -f rc.cpp
