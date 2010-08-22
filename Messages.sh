#! /usr/bin/env bash
find . -name "*.cpp" -print > files 
find . -name "*.cc" -print >> files
find . -name "*.h" -print >> files
toolbars=`find . -name "*.toolbar.tgz"`
touch extrafiles
touch extrafiles2
for toolbar in $toolbars; do 
   basename=`echo "$toolbar" | sed "s/.*\///"| sed "s/\\..*$//"`
   tar Oxfz $toolbar "$basename.toolbar" >> extrafiles
   tar Oxfz $toolbar "$basename.actions" >> extrafiles2
done
#cat data/config/actions.rc >> extrafiles2
$EXTRACTRC `find . -name "*.ui"` >> rc.cpp 
$EXTRACTRC `find . -name "*.rc"        \
            | grep -v /UNPORTED/data/` >> rc.cpp 
$EXTRACTRC `find . -name "*.kmdr"` >> rc.cpp 
$EXTRACTRC `find . -name "*.kcfg"` >> rc.cpp
#cat data/chars | perl -e 'while(<STDIN>) { chomp ; s/\"/\\\"/ ; print "i18n(\"$_\");\n"; }' >> rc.cpp 
$EXTRACTRC extrafiles >> rc.cpp 
cat extrafiles | perl -e 'while(<STDIN>) { if (/\<action .* text="(.*)"/) { print "i18n(\"$1\");\n"; }}' >> rc.cpp
cat extrafiles2 | perl -e 'while(<STDIN>) { if (/\<action .* text="(.*)"/) { print "i18n(\"$1\");\n"; }}' >> rc.cpp
$EXTRACTATTR extrafiles2 --attr=action,tooltip >> rc.cpp 
$EXTRACTRC extrafiles2 --tag-group=none --tag=tooltip >> rc.cpp 
touch tips.cpp
#(cd data && $PREPARETIPS >> ../tips.cpp)
find . -name "*.tag" -print | xargs cat | perl -e 'while(<STDIN>) { if (/\<tag .* comment="(.*)"/) { print "i18n(\"$1\");\n"; }}' >> rc.cpp
$XGETTEXT rc.cpp tips.cpp `cat files` -o $podir/quanta.pot
rm -f extrafiles extrafiles2 tips.cpp files dirs

