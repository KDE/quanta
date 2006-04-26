INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)

kde_module_LTLIBRARIES = libkdevtemplatestree.la
libkdevtemplatestree_la_LDFLAGS = -avoid-version -module -no-undefined \
	-L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
libkdevtemplatestree_la_LIBADD = -lkdevquanta -lkdevelop
libkdevtemplatestree_la_SOURCES = templatestreepart.cpp \
	templatestreeglobalconfigbase.ui templatestreeglobalconfig.cpp templatestreeprojectconfigbase.ui \
	templatestreeprojectconfig.cpp templatedirform.ui templatestreeview.cpp newtemplatedirdlg.cpp \
	quantapropertiespage.ui

METASOURCES = AUTO

servicedir = $(kde_servicesdir)
service_DATA = kdevtemplatestree.desktop

rcdir = $(kde_datadir)/kdevtemplatestree
rc_DATA = kdevtemplatestree.rc

appsharedir = $(kde_datadir)/kdevtemplatestree
appshare_DATA =
noinst_HEADERS = templatestreeview.h newtemplatedirdlg.h
