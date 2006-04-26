INCLUDES = -I$(top_srcdir)/lib -I$(kde_includes)/kdevelop/interfaces \
	-I$(kde_includes)/kdevelop/util $(all_includes)

kde_module_LTLIBRARIES = libkdevquantaproject.la
libkdevquantaproject_la_LDFLAGS = -L$(top_builddir)/lib -avoid-version -module -no-undefined \
	$(all_libraries) $(KDE_PLUGIN)
libkdevquantaproject_la_LIBADD = -lkdevquanta	-lkdevelop
libkdevquantaproject_la_SOURCES = quantaprojectpart.cpp quantaprojectwidget.cpp \
		quantaprojectprojectconfigbase.ui quantaprojectprojectconfig.cpp browserpopupmenu.cpp

METASOURCES = AUTO

servicedir = $(kde_servicesdir)
service_DATA = kdevquantaproject.desktop

rcdir = $(kde_datadir)/kdevquantaproject
rc_DATA = kdevquantaproject.rc

appsharedir = $(kde_datadir)/kdevquantaproject
appshare_DATA =
noinst_HEADERS = browserpopupmenu.h
