#!/bin/bash
#Previews the current document in Mozilla/Netscape/Opera, depending
#on the last two arguments (browsername, newtab command).
#Might be adjusted in the feature to support more browsers.
pid=$1
currentURL=""
if test $pid == "unique"
then
  currentURL=`dcop quanta WindowManagerIf saveCurrentFile`
  shift
else
  currentURL=`dcop quanta-$pid WindowManagerIf saveCurrentFile`
fi
browser=$2
newTab=$3
if test $currentURL
then
  if ! $browser -remote "openURL($currentURL,$newTab)"
  then
    $browser $currentURL
  fi
fi