#!/bin/sh
files=`find . -name \*.rc -o -name \*.ui`
if [ "x$files" != "x" ]; then
    $EXTRACTRC $files >> rc.cpp
fi
$XGETTEXT `find . -name \*.cc -o -name \*.cpp -o -name \*.h` -o $podir/kdevphpformatter.pot
rm -f rc.cpp
