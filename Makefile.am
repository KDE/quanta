SUBDIRS = data utility dialogs components parsers treeviews messages plugins parts project src scripts

#extract messages from sources, toolbars, data files and the toplevel lib directory
messages: rc.cpp
	find . -name "*.cpp" -print > files ;\
	find . -name "*.cc" -print >> files ;\
	find . -name "*.h" -print >> files ;\
	find ../lib -name "*.h" -print >> files ;\
	find ../lib -name "*.cpp" -print >> files ;\
	toolbars=`find . -name "*.toolbar.tgz"`;\
	for toolbar in $$toolbars; do \
	    tar Oxfz $$toolbar >> extrafiles ; \
	done ;\
	cat data/config/actions.rc >> extrafiles; \
	$(EXTRACTRC) `find . -name "*.ui"` >> rc.cpp ;\
	$(EXTRACTRC) `find . -name "*.rc"` >> rc.cpp ;\
	$(EXTRACTRC) `find . -name "*.kmdr"` >> rc.cpp ;\
	cat data/chars | perl -e 'while(<STDIN>) { chomp ; s/\"/\\\"/ ; print "i18n(\"$$_\");\n"; }' >> rc.cpp ;\
	$(EXTRACTRC) extrafiles >> rc.cpp ;\
	cat extrafiles | perl -e 'while(<STDIN>) { if (/\<action .* text="(.*)"/) { print "i18n(\"$$1\");\n"; }}' | sed -e 's/\&amp;/\&/g' >> rc.cpp
	(cd data && $(PREPARETIPS) >> ../tips.cpp)
	find . -name "*.tag" -print | xargs cat | perl -e 'while(<STDIN>) { if (/\<tag .* comment="(.*)"/) { print "i18n(\"$$1\");\n"; }}' >> rc.cpp
	$(XGETTEXT) rc.cpp tips.cpp `cat files` -o $(podir)/quanta.pot
	rm -f extrafiles tips.cpp files dirs
