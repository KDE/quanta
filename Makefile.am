
if include_qextmdi
KMDI_SUBDIR = kmdi
endif

#disable kmdi unconditionally for now
#SUBDIRS = data dialogs messages parser parts plugins project tagdialogs toolbar treeviews widgets $(KMDI_SUBDIR)
SUBDIRS = data dialogs messages parser parts plugins project tagdialogs toolbar treeviews widgets

if BUILD_WYSIWYG
LIB_KAFKA = $(top_builddir)/quanta/parts/kafka/libkafkalibrary.la
endif

# Make merging simple
LIB_KMDI =

bin_PROGRAMS = quanta

quanta_SOURCES = quantacommon.cpp quanta_init.cpp quantaview.cpp quantadoc.cpp quantaview_slots.cpp main.cpp document.cpp kqapp.cpp qextfileinfo.cpp quanta.cpp dtds.cpp dcopwindowmanagerif.skel

quanta_LDADD = $(LIB_KAFKA) $(LIB_KMDI) $(top_builddir)/quanta/project/libproject.la $(top_builddir)/quanta/plugins/libplugins.la $(top_builddir)/quanta/widgets/libwidgets.la $(top_builddir)/quanta/parser/libparser.la $(top_builddir)/quanta/treeviews/libtreeviews.la $(top_builddir)/quanta/toolbar/libtoolbar.la $(top_builddir)/quanta/tagdialogs/libtagdialogs.la $(top_builddir)/quanta/dialogs/libdialogs.la $(top_builddir)/quanta/messages/libmessages.la $(top_builddir)/quanta/plugins/php4dbg/libphp4dbg.la $(top_builddir)/quanta/plugins/php4dbg/dbgbase/libdbgbase.la $(top_builddir)/quanta/plugins/framewizard/libframewizard.la $(top_builddir)/quanta/plugins/csseditor/libcsseditor.la $(top_builddir)/quanta/plugins/php3dbg/libphp3dbg.la -lkatepartinterfaces $(LIBXML_LIBS) $(LIB_KABC) $(LIB_KSPELL) $(LIB_KHTML)

quanta_METASOURCES = AUTO

# the library search path.
quanta_LDFLAGS = $(all_libraries) $(KDE_RPATH)

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
	cat data/actions.rc >> extrafiles; \
	$(EXTRACTRC) `find $$dirs -maxdepth 1 -name "*.ui"` >> rc.cpp ;\
	$(EXTRACTRC) `find $$dirs -maxdepth 1 -name "*.rc"` >> rc.cpp ;\
	$(EXTRACTRC) `find $$dirs -maxdepth 1 -name "*.kmdr"` >> rc.cpp ;\
	cat data/chars | perl -e 'while(<STDIN>) { chomp ; s/\"/\\\"/ ; print "i18n(\"$$_\");\n"; }' >> rc.cpp ;\
	$(EXTRACTRC) extrafiles >> rc.cpp ;\
	cat extrafiles | perl -e 'while(<STDIN>) { if (/\<action .* text="(.*)"/) { print "i18n(\"$$1\");\n"; }}' >> rc.cpp
	(cd data && $(PREPARETIPS) >> ../tips.cpp)
	$(XGETTEXT) rc.cpp tips.cpp `cat files` -o $(podir)/quanta.pot
	rm -f extrafiles tips.cpp files dirs 

# this is where the kdelnk file will go
kdelnkdir = $(kde_appsdir)/Development
kdelnk_DATA = quanta.desktop

# this is where the XML-GUI resource file goes
rcdir = ${quanta_datadir}
rc_DATA = quantaui.rc

if QUANTAUIRC_HOOK
install-data-hook:
	mv $(DESTDIR)$(rcdir)/quantaui.rc $(DESTDIR)$(rcdir)/$(PACKAGE)ui.rc || true
else
install-data-hook:
#no hook        
endif        

if BUILD_WYSIWYG
KAFKAINCLUDE = -I$(top_builddir)/quanta/parts/kafka
endif

INCLUDES = -I$(top_srcdir)/quanta/dialogs -I$(top_srcdir)/quanta/plugins -I$(top_srcdir)/quanta/plugins/framewizard -I$(top_builddir)/quanta/plugins/csseditor -I$(top_srcdir)/quanta/tagdialogs -I$(top_srcdir)/quanta/toolbar -I$(top_builddir)/quanta/toolbar -I$(top_builddir)/quanta/tagdialogs -I$(top_builddir)/quanta/plugins -I$(top_builddir)/quanta/dialogs -I$(top_builddir)/quanta/plugins/framewizard $(KAFKAINCLUDE) $(KMDI_INCLUDES) $(all_includes)



