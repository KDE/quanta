#1########################################################!@#$
####   Makefile.am generated automatically by KDEStudio   ####
#### WARNING! All changes made in this part will be lost! ####
##############################################################
SUBDIRS = toolbar data widgets treeviews tagdialogs project parser dialogs kwrite 
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
		-lX11 $\
		-lz\
		-lpng -lz -lm\
		-ljpeg\
		-lqt $ $ -lXext $ $\
		-lkdecore\
		-lkdeui\
		-lkio\
		-lkfile\
		-lkhtml\
		-lkspell\
		-ltiff -ljpeg -lz
LDFLAGS =   -lktexteditor
INCLUDES = $(all_includes)
CXXFLAGS = -g -Wall 
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
