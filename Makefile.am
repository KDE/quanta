####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = quanta

## INCLUDES were found outside kdevelop specific part

quanta_SOURCES = quantacommon.cpp quanta_init.cpp quantaview.cpp quantadoc.cpp quantaview_slots.cpp main.cpp document.cpp kqapp.cpp qextfileinfo.cpp quanta.cpp dcopwindowmanagerif.skel
quanta_LDADD = $(top_builddir)/quanta/project/libproject.a $(top_builddir)/quanta/plugins/libplugins.a $(top_builddir)/quanta/widgets/libwidgets.a $(top_builddir)/quanta/parser/libparser.a $(top_builddir)/quanta/treeviews/libtreeviews.a $(top_builddir)/quanta/toolbar/libtoolbar.a $(top_builddir)/quanta/tagdialogs/libtagdialogs.a $(top_builddir)/quanta/dialogs/libdialogs.a  $(top_builddir)/quanta/messages/libmessages.a $(top_builddir)/quanta/plugins/php4dbg/libphp4dbg.a $(top_builddir)/quanta/plugins/php4dbg/dbgbase/libdbgbase.a $(top_builddir)/quanta/plugins/php3dbg/libphp3dbg.a -lkatepartinterfaces $(LIB_KABC) $(LIB_KSPELL) $(LIB_KHTML)

SUBDIRS = data messages dialogs tagdialogs toolbar treeviews parser widgets plugins project parts

####### kdevelop will overwrite this part!!! (end)############
quanta_METASOURCES = AUTO

# the library search path.
quanta_LDFLAGS = $(all_libraries) $(KDE_RPATH)

messages: rc.cpp
	$(EXTRACTRC) `find . -name "*.ui"` >> rc.cpp
	$(EXTRACTRC) `find . -name "*.rc"` >> rc.cpp
	$(XGETTEXT) `find . -name "*.cpp"` -o $(podir)/quanta.pot
 
KDE_ICON = quanta
 
# this is where the kdelnk file will go
kdelnkdir = $(kde_appsdir)/Development
kdelnk_DATA = quanta.desktop
 
# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/quanta
rc_DATA = quantaui.rc


INCLUDES = -I$(top_srcdir)/quanta/dialogs -I$(top_srcdir)/quanta/plugins -I$(top_srcdir)/quanta/tagdialogs -I$(top_srcdir)/quanta/toolbar -I$(top_builddir)/quanta/toolbar -I$(top_builddir)/quanta/tagdialogs -I$(top_builddir)/quanta/plugins -I$(top_builddir)/quanta/dialogs $(all_includes)

AM_CFLAGS = 
AM_CXXFLAGS = 
AM_FFLAGS = 
