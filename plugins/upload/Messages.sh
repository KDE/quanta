#!/bin/sh
files=`find . -name \*.rc -o -name \*.ui -o -name \*.kcfg`
if [ "x$files" != "x" ]; then
	$EXTRACTRC $files >> rc.cpp
fi
$XGETTEXT `find . -name \*.cc -o -name \*.cpp -o -name \*.h | grep -v '/tests/'` -o $podir/kdevupload.pot
rm -f rc.cpp
