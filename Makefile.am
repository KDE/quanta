
if include_qextmdi
KMDI_SUBDIR = kmdi
endif

#disable kmdi unconditionally for now
#SUBDIRS = data dialogs messages parser parts plugins project tagdialogs toolbar treeviews widgets $(KMDI_SUBDIR)
SUBDIRS = data dialogs messages parser parts plugins project tagdialogs toolbar treeviews widgets

bin_PROGRAMS = quanta

quanta_SOURCES = quantacommon.cpp quanta_init.cpp quantaview.cpp quantadoc.cpp quantaview_slots.cpp main.cpp document.cpp kqapp.cpp qextfileinfo.cpp quanta.cpp dcopwindowmanagerif.skel

if BUILD_WYSIWYG
LIB_KAFKA = $(top_builddir)/quanta/parts/kafka/libkafkalibrary.la
endif

# Make merging simple
LIB_KMDI =

quanta_LDADD = $(LIB_KAFKA) $(LIB_KMDI) $(top_builddir)/quanta/project/libproject.la $(top_builddir)/quanta/plugins/libplugins.la $(top_builddir)/quanta/widgets/libwidgets.la $(top_builddir)/quanta/parser/libparser.la $(top_builddir)/quanta/treeviews/libtreeviews.la $(top_builddir)/quanta/toolbar/libtoolbar.la $(top_builddir)/quanta/tagdialogs/libtagdialogs.la $(top_builddir)/quanta/dialogs/libdialogs.la $(top_builddir)/quanta/messages/libmessages.la $(top_builddir)/quanta/plugins/php4dbg/libphp4dbg.la $(top_builddir)/quanta/plugins/php4dbg/dbgbase/libdbgbase.la $(top_builddir)/quanta/plugins/framewizard/libframewizard.la $(top_builddir)/quanta/plugins/csseditor/libcsseditor.la $(top_builddir)/quanta/plugins/php3dbg/libphp3dbg.la -lkatepartinterfaces $(LIBXML_LIBS) $(LIB_KABC) $(LIB_KSPELL) $(LIB_KHTML)

quanta_METASOURCES = AUTO

# the library search path.
quanta_LDFLAGS = $(all_libraries) $(KDE_RPATH)

messages: rc.cpp
	$(EXTRACTRC) `find . -name "*.ui"` >> rc.cpp
	$(EXTRACTRC) `find . -name "*.rc"` >> rc.cpp
	$(EXTRACTRC) `find . -name "*.kmdr"` >> rc.cpp
	$(EXTRACTRC) $(top_srcdir)/quanta/data/chars >> rc.cpp
	$(XGETTEXT) `find . -name "*.cpp"` -o $(podir)/quanta.pot

KDE_ICON = quanta

# this is where the kdelnk file will go
kdelnkdir = $(kde_appsdir)/Development
kdelnk_DATA = quanta.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/quanta
rc_DATA = quantaui.rc

if BUILD_WYSIWYG
KAFKAINCLUDE = -I$(top_builddir)/quanta/parts/kafka
endif

INCLUDES = -I$(top_srcdir)/quanta/dialogs -I$(top_srcdir)/quanta/plugins -I$(top_srcdir)/quanta/plugins/framewizard -I$(top_builddir)/quanta/plugins/csseditor -I$(top_srcdir)/quanta/tagdialogs -I$(top_srcdir)/quanta/toolbar -I$(top_builddir)/quanta/toolbar -I$(top_builddir)/quanta/tagdialogs -I$(top_builddir)/quanta/plugins -I$(top_builddir)/quanta/dialogs -I$(top_builddir)/quanta/plugins/framewizard $(KAFKAINCLUDE) $(KMDI_INCLUDES) $(all_includes)



