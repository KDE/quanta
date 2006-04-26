INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)

kde_module_LTLIBRARIES = libkdevtagdialogs.la
libkdevtagdialogs_la_LDFLAGS = -avoid-version -module -no-undefined \
	-L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
libkdevtagdialogs_la_LIBADD = -lkdevquanta -lkdevelop
libkdevtagdialogs_la_SOURCES = tagdialogspart.cpp colorcombo.cpp listdlg.cpp \
	pictureview.cpp tagdialog.cpp tagimgdlg.cpp tagimgdlgdata.cpp tagmaildlg.cpp tagmail.ui \
	tagmiscdlg.cpp tagmisc.ui tagquicklistdlg.cpp tagquicklistdlgdata.cpp tagwidget.cpp \
	tagxml.cpp attributes.cpp

METASOURCES = AUTO

servicedir = $(kde_servicesdir)
service_DATA = kdevtagdialogs.desktop

servicetypedir = $(kde_servicetypesdir)
servicetype_DATA = kdeveloptagdialogs.desktop

rcdir = $(kde_datadir)/kdevtagdialogs
rc_DATA = kdevtagdialogs.rc

appsharedir = $(kde_datadir)/kdevtagdialogs
appshare_DATA =
noinst_HEADERS = tagdialogspart.h colorcombo.h listdlg.h pictureview.h \
	tagdialog.h tagimgdlg.h tagmaildlg.h tagmiscdlg.h tagquicklistdlg.h tagwidget.h \
	tagxml.h attributes.h

