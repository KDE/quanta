#1########################################################!@#$
####   Makefile.am generated automatically by KDEStudio   ####
#### WARNING! All changes made in this part will be lost! ####
##############################################################
SUBDIRS = plugins messages toolbar data widgets treeviews tagdialogs project parser dialogs kwrite 
bin_PROGRAMS = quanta


quanta_LDFLAGS = $(all_libraries)
quanta_LDADD = \
		$(top_srcdir)/quanta/kwrite/libkwrite.a\
		$(top_srcdir)/quanta/kwrite/highlight/libhighlight.a\
		$(top_srcdir)/quanta/dialogs/libdialogs.a\
		$(top_srcdir)/quanta/parser/libparser.a\
		$(top_srcdir)/quanta/project/libproject.a\
		$(top_srcdir)/quanta/tagdialogs/libtagdialogs.a\
		$(top_srcdir)/quanta/treeviews/libtreeviews.a\
		$(top_srcdir)/quanta/widgets/libwidgets.a\
		$(top_srcdir)/quanta/toolbar/libtoolbar.a\
		$(top_srcdir)/quanta/messages/libmessages.a\
		$(top_srcdir)/quanta/plugins/php4dbg/libphp4dbg.a\
		$(top_srcdir)/quanta/plugins/php4dbg/dbgbase/libdbgbase.a\
		$(top_srcdir)/quanta/plugins/php3dbg/libphp3dbg.a\
		-lSM -lICE\
		-lX11 $\
		-lpng -lz -lm\
		-ljpeg\
		-lqt $ $ -lXext $ $\
		-lkdeui\
		-lkfile\
		-lkhtml\
		-lkspell
LDFLAGS =   -lktexteditor
INCLUDES = $(all_includes)\
		-I$(top_srcdir)/quanta/kwrite\
		-I$(top_srcdir)/quanta/kwrite/highlight
CXXFLAGS = -Wall 
DEFS = -I. -I$(srcdir) -I$(top_srcdir)
quanta_METASOURCES = AUTO
quanta_SOURCES = \
		document.cpp\
		main.cpp\
		qextfileinfo.cpp\
		quanta.cpp\
		quanta_init.cpp\
		quantadoc.cpp\
		quantaview.cpp\
		quantaview_init.cpp\
		quantaview_slots.cpp\
		kqapp.cpp
EXTRA_DIST = \
		document.cpp\
		document.h\
		main.cpp\
		qextfileinfo.cpp\
		qextfileinfo.h\
		quanta.cpp\
		quanta.desktop\
		quanta.h\
		quanta_init.cpp\
		quantadoc.cpp\
		quantadoc.h\
		quantaview.cpp\
		quantaview.h\
		quantaview_init.cpp\
		quantaview_slots.cpp\
		resource.h\
		quantaui.rc\
		kqapp.cpp\
		kqapp.h
##############################################################
####                  END KDEStudio part                  ####
#2########################################################!@#$

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
