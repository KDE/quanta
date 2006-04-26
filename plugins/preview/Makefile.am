INCLUDES = -I$(kde_includes)/kdevelop/widgets \
	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)

kde_module_LTLIBRARIES = libkdevhtmlpreview.la
libkdevhtmlpreview_la_LDFLAGS = -avoid-version -module -no-undefined \
	$(all_libraries) $(KDE_PLUGIN)
libkdevhtmlpreview_la_LIBADD = -lkdevwidgets -lkdevelop $(LIB_KHTML)
libkdevhtmlpreview_la_SOURCES = htmlpreviewpart.cpp \
	htmlpreviewglobalconfigbase.ui htmlpreviewglobalconfig.cpp htmlpreviewprojectconfigbase.ui \
	htmlpreviewprojectconfig.cpp htmlpart.cpp

METASOURCES = AUTO

servicedir = $(kde_servicesdir)
service_DATA = kdevhtmlpreview.desktop

rcdir = $(kde_datadir)/kdevhtmlpreview
rc_DATA = kdevhtmlpreview.rc

appsharedir = $(kde_datadir)/kdevhtmlpreview
appshare_DATA =
noinst_HEADERS = htmlpart.h
