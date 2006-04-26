INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)

kde_module_LTLIBRARIES = libkdevprojecttree.la
libkdevprojecttree_la_LDFLAGS = -avoid-version -module -no-undefined \
	-L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
libkdevprojecttree_la_LIBADD = -lkdevquanta -lkdevelop
libkdevprojecttree_la_SOURCES = projecttreepart.cpp projecttreewidget.cpp \
	projecttreeglobalconfigbase.ui projecttreeglobalconfig.cpp projecttreeprojectconfigbase.ui \
	projecttreeprojectconfig.cpp projecttreeview.cpp

METASOURCES = AUTO

servicedir = $(kde_servicesdir)
service_DATA = kdevprojecttree.desktop

rcdir = $(kde_datadir)/kdevprojecttree
rc_DATA = kdevprojecttree.rc

appsharedir = $(kde_datadir)/kdevprojecttree
appshare_DATA =
noinst_HEADERS = projecttreeview.h
