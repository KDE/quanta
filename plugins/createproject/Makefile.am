INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)

kde_module_LTLIBRARIES = libkdevcreatequantaproject.la
libkdevcreatequantaproject_la_LDFLAGS = -L$(top_builddir)/lib -avoid-version -module -no-undefined \
	$(all_libraries) $(KDE_PLUGIN)
libkdevcreatequantaproject_la_LIBADD = -lkdevelop -lkdevquanta
libkdevcreatequantaproject_la_SOURCES = createquantaprojectpart.cpp \
	simplefiletree.cpp simpletreeviewitem.cpp firstwizardpagebase.ui firstwizardpage.cpp \
	localimportwizardpagebase.ui localimportwizardpage.cpp secondwizardpagebase.ui secondwizardpage.cpp \
	webimportwizardpagebase.ui webimportwizardpage.cpp

METASOURCES = AUTO

servicedir = $(kde_servicesdir)
service_DATA = kdevcreatequantaproject.desktop

rcdir = $(kde_datadir)/kdevcreatequantaproject
rc_DATA = kdevcreatequantaproject.rc quanta-project.template

appsharedir = $(kde_datadir)/kdevcreatequantaproject
appshare_DATA =
noinst_HEADERS = simplefiletree.h simpletreeviewitem.h firstwizardpage.h \
	localimportwizardpage.h secondwizardpage.h webimportwizardpage.h

SUBDIRS = pics
