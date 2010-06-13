#!/bin/sh
$EXTRACTRC `find . -name \*.rc` `find . -name \*.ui` >> rc.cpp
$XGETTEXT `find . -name \*.cpp -o -name \*.h` -o $podir/kdevcrossfire.pot
rm -f rc.cpp
