if BUILD_WYSIWYG
LIB_KAFKA = $(top_builddir)/quanta/parts/kafka/libkafkalibrary.la
KAFKAINCLUDE = -I$(top_srcdir)/quanta/parts/kafka -I$(top_builddir)/quanta/parts/kafka
endif

# Make merging simple
LIB_KMDI = 

bin_PROGRAMS = quanta_be

quanta_be_SOURCES = quanta_init.cpp quantaview.cpp quantadoc.cpp quantaview_slots.cpp main.cpp document.cpp kqapp.cpp quanta.cpp dcopwindowmanagerif.skel dcopsettingsif.skel dcopquantaif.skel dcopsettings.cpp dtds.cpp dcopquanta.cpp


quanta_be_LDADD = $(top_builddir)/quanta/project/libproject.la \
	$(top_builddir)/quanta/plugins/libplugins.la \
	$(top_builddir)/quanta/parsers/libparser.la \
	$(top_builddir)/quanta/parsers/dtd/libdtdparser.la \
	$(top_builddir)/quanta/treeviews/libtreeviews.la \
	$(top_builddir)/quanta/dialogs/libdialogs.la \
	$(top_builddir)/quanta/components/phpdebug/libphpdebug.la \
	$(top_builddir)/quanta/dialogs/tagdialogs/libtagdialogs.la \
	$(top_builddir)/quanta/dialogs/settings/libsettingsdialogs.la \
	$(top_builddir)/quanta/messages/libmessages.la \
	$(top_builddir)/quanta/components/framewizard/libframewizard.la \
	$(top_builddir)/quanta/components/csseditor/libcsseditor.la \
	$(top_builddir)/quanta/components/tableeditor/libtableeditor.la \
	$(top_builddir)/quanta/parts/preview/libpreview.la \
	$(top_builddir)/quanta/utility/libutility.la \
	$(top_builddir)/lib/libquantamodule.la \
	$(top_builddir)/lib/compatibility/libcompat.la \
	-lkatepartinterfaces $(LIB_KAFKA) $(LIB_KMDI) $(LIBXML_LIBS) $(LIB_KABC) $(LIB_KSPELL) $(LIB_KHTML)

AM_CPPFLAGS = -I$(top_srcdir)/quanta/dialogs \
	-I$(top_srcdir)/quanta/plugins \
	-I$(top_srcdir)/quanta/components/framewizard \
	-I$(top_srcdir)/quanta/components/csseditor \
	-I$(top_srcdir)/quanta/dialogs/tagdialogs \
	-I$(top_srcdir)/quanta/dialogs/settings \
	-I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/treeviews \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/parsers/dtd \
	-I$(top_srcdir)/quanta/parts/preview \
	-I$(top_srcdir)/quanta/messages \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/lib/compatibility \
	-I$(top_builddir)/quanta/dialogs \
	-I$(top_builddir)/quanta/dialogs/tagdialogs \
	-I$(top_builddir)/quanta/dialogs/settings \
	-I$(top_srcdir)/quanta/components/phpdebug \
	-I$(top_builddir)/quanta/components/csseditor \
	-I$(top_builddir)/quanta/components/framewizard \
	-I$(top_builddir)/quanta/components/tableeditor \
	-I$(top_builddir)/quanta/plugins \
	$(KAFKAINCLUDE) $(KMDI_INCLUDES) $(all_includes)

quanta_be_METASOURCES = AUTO

# the library search path.
quanta_be_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# some parts got own messages targets. No need to duplicate their messages
messages: rc.cpp
	find . -type d | grep -v CVS | sed -e 's,$$,/,' > dirs
	mfs=`find . -name Makefile.am | xargs egrep -l '^messages:'` ;\
	for dir in $$mfs; do \
	  if test ! "$$dir" = "./Makefile.am"; then \
	    dir=`dirname $$dir` ;\
	    egrep -v "^$$dir" dirs > dirs.new && mv dirs.new dirs ;\
	  fi ;\
	done
	egrep -v "/tests" dirs > dirs.new && mv dirs.new dirs ;\
	dirs=`cat dirs` ;\
	find $$dirs -maxdepth 1 -name "*.cpp" -print > files ;\
	find $$dirs -maxdepth 1 -name "*.cc" -print >> files ;\
	find $$dirs -maxdepth 1 -name "*.h" -print >> files ;\
	toolbars=`find . -name "*.toolbar.tgz"`;\
	for toolbar in $$toolbars; do \
	 tar Oxfz $$toolbar  >> extrafiles; \
	done ;\
	cat ../data/config/actions.rc >> extrafiles; \
	$(EXTRACTRC) `find $$dirs -maxdepth 1 -name "*.ui"` >> rc.cpp ;\
	$(EXTRACTRC) `find $$dirs -maxdepth 1 -name "*.rc"` >> rc.cpp ;\
	$(EXTRACTRC) `find $$dirs -maxdepth 1 -name "*.kmdr"` >> rc.cpp ;\
	cat ../data/chars | perl -e 'while(<STDIN>) { chomp ; s/\"/\\\"/ ; print "i18n(\"$$_\");\n"; }' >> rc.cpp ;\
	$(EXTRACTRC) extrafiles >> rc.cpp ;\
	cat extrafiles | perl -e 'while(<STDIN>) { if (/\<action .* text="(.*)"/) { print "i18n(\"$$1\");\n"; }}' >> rc.cpp
	(cd ../data && $(PREPARETIPS) >> ../src/tips.cpp)
	$(XGETTEXT) rc.cpp tips.cpp `cat files` -o $(podir)/quanta.pot
	rm -f extrafiles tips.cpp files dirs 

# this is where the kdelnk file will go
kdelnkdir = $(kde_appsdir)/Development
kdelnk_DATA = quanta_be.desktop
