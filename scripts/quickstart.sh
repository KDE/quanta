#!/bin/bash
pid=$1
dcopCommand=""
if test $pid == "unique"
then
  dcopCommand="dcop quanta"
else
  dcopCommand="dcop quanta-$pid"
fi
encoding=`$dcopCommand SettingsIf encoding`
a=`kmdr-executor $2 &`
sleep 2
pid2=$!
dcop kmdr-executor-$pid2 KommanderIf setChecked CBMetaChar true
dcop kmdr-executor-$pid2 KommanderIf setCurrentListItem ComboMetachar "$encoding"

wait $pid2
echo $a