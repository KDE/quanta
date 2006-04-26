INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)

kde_module_LTLIBRARIES = libkdevquantafilestree.la
libkdevquantafilestree_la_LDFLAGS = -avoid-version -module -no-undefined \
	-L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
libkdevquantafilestree_la_LIBADD = -lkdevquanta -lkdevelop
libkdevquantafilestree_la_SOURCES = quantafilestreepart.cpp \
	quantafilestreeglobalconfigbase.ui quantafilestreeglobalconfig.cpp quantafilestreeprojectconfigbase.ui \
	quantafilestreeprojectconfig.cpp filestreeview.cpp

METASOURCES = AUTO

servicedir = $(kde_servicesdir)
service_DATA = kdevquantafilestree.desktop

rcdir = $(kde_datadir)/kdevquantafilestree
rc_DATA = kdevquantafilestree.rc

appsharedir = $(kde_datadir)/kdevquantafilestree
appshare_DATA =
noinst_HEADERS = filestreeview.h
