####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = quanta

INCLUDES = $(all_includes)

quanta_SOURCES = quanta_init.cpp quantaview.cpp quantadoc.cpp quantaview_slots.cpp quantaview_init.cpp main.cpp document.cpp kqapp.cpp qextfileinfo.cpp quanta.cpp 
quanta_LDADD   = ./project/libproject.a ./treeviews/libtreeviews.a ./tagdialogs/libtagdialogs.a -lkatepart -lkabc $(top_srcdir)/quanta/dialogs/libdialogs.a $(top_srcdir)/quanta/parser/libparser.a $(top_srcdir)/quanta/project/libproject.a $(top_srcdir)/quanta/tagdialogs/libtagdialogs.a $(top_srcdir)/quanta/treeviews/libtreeviews.a $(top_srcdir)/quanta/widgets/libwidgets.a $(top_srcdir)/quanta/toolbar/libtoolbar.a $(top_srcdir)/quanta/messages/libmessages.a $(top_srcdir)/quanta/plugins/php4dbg/libphp4dbg.a $(top_srcdir)/quanta/plugins/php4dbg/dbgbase/libdbgbase.a $(top_srcdir)/quanta/plugins/php3dbg/libphp3dbg.a $(LIB_KSPELL) $(LIB_KFILE) $(LIB_KHTML) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) -lXext -lX11 $(LIBSOCKET)

SUBDIRS = data messages dialogs tagdialogs toolbar treeviews parser widgets plugins project 

EXTRA_DIST = quanta.cpp resource.h kqapp.h qextfileinfo.cpp kqapp.cpp document.cpp quantadoc.h quantaui.rc document.h main.cpp quantaview_init.cpp quantaview_slots.cpp quanta.desktop quantadoc.cpp quanta.h qextfileinfo.h quantaview.h quantaview.cpp quanta_init.cpp 

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
kdelnkdir   = $(kde_appsdir)/Development
kdelnk_DATA = quanta.desktop
 
# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/quanta
rc_DATA = quantaui.rc

