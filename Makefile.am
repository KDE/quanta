#1########################################################!@#$
####   Makefile.am generated automatically by KDEStudio   ####
#### WARNING! All changes made in this part will be lost! ####
##############################################################
SUBDIRS = plugins messages phpdebug toolbar data widgets treeviews tagdialogs project parser dialogs kwrite 
bin_PROGRAMS = quanta


quanta_LDFLAGS = $(all_libraries) $(KDE_RPATH)
quanta_LDADD = \
		$(top_builddir)/quanta/kwrite/libkwrite.a\
		$(top_builddir)/quanta/kwrite/highlight/libhighlight.a\
		$(top_builddir)/quanta/dialogs/libdialogs.a\
		$(top_builddir)/quanta/parser/libparser.a\
		$(top_builddir)/quanta/project/libproject.a\
		$(top_builddir)/quanta/tagdialogs/libtagdialogs.a\
		$(top_builddir)/quanta/treeviews/libtreeviews.a\
		$(top_builddir)/quanta/widgets/libwidgets.a\
		$(top_builddir)/quanta/toolbar/libtoolbar.a\
		$(top_builddir)/quanta/phpdebug/libphpdebug.a\
		$(top_builddir)/quanta/messages/libmessages.a\
		$(LIB_KHTML) $(LIB_KFILE) -lkspell

LDFLAGS =  -s -lktexteditor
INCLUDES = $(all_includes)\
		-I$(top_srcdir)/quanta/plugins/weblint
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
		quantaview_slots.cpp
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
		resource.h
##############################################################
####                  END KDEStudio part                  ####
#2########################################################!@#$

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Office/
	$(INSTALL_DATA) $(srcdir)/quanta.desktop $(kde_appsdir)/Office/quanta.desktop

uninstall-local:
	-rm -f $(kde_appsdir)/Office/quanta.desktop
