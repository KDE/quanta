INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)

kde_module_LTLIBRARIES = libkdevstructuretree.la
libkdevstructuretree_la_LDFLAGS = -avoid-version -module -no-undefined \
	-L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
libkdevstructuretree_la_LIBADD = -lkdevquanta -lkdevelop
libkdevstructuretree_la_SOURCES = structuretreepart.cpp structuretreewidget.cpp \
	structuretreeglobalconfigbase.ui structuretreeglobalconfig.cpp structuretreeprojectconfigbase.ui \
	structuretreeprojectconfig.cpp structtreeitem.cpp groupswidget.cpp groupstreeitem.cpp

METASOURCES = AUTO

servicedir = $(kde_servicesdir)
service_DATA = kdevstructuretree.desktop

rcdir = $(kde_datadir)/kdevstructuretree
rc_DATA = kdevstructuretree.rc

appsharedir = $(kde_datadir)/kdevstructuretree
appshare_DATA =
noinst_HEADERS = structtreeitem.h groupswidget.h groupstreeitem.h






