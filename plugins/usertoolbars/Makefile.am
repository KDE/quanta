AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)

kde_module_LTLIBRARIES = libkdevusertoolbars.la
libkdevusertoolbars_la_LDFLAGS = -module -avoid-version -no-undefined -L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
libkdevusertoolbars_la_LIBADD = -lkdevquanta -lkdevelop -lkdevextras -lknewstuff 
libkdevusertoolbars_la_SOURCES = usertoolbarspart.cpp \
	usertoolbarsglobalconfigbase.ui usertoolbarsglobalconfig.cpp toolbarxmlgui.cpp toolbartabwidget.cpp \
	toolbarguibuilder.cpp usertoolbar.cpp askforsavedlg.cpp maildialogbase.ui maildialog.cpp \
	newtoolbarstuff.cpp actionconfigdialogbase.ui actionconfigdialog.cpp

METASOURCES = AUTO

servicedir = $(kde_servicesdir)
service_DATA = kdevusertoolbars.desktop

rcdir = $(kde_datadir)/kdevusertoolbars
rc_DATA = kdevusertoolbars.rc

globaldir = $(kde_datadir)/kdevusertoolbars
global_DATA = global

quantadir = $(kde_datadir)/quanta
quanta_DATA = global

appsharedir = $(kde_datadir)/kdevusertoolbars
appshare_DATA =
noinst_HEADERS = toolbarguibuilder.h askforsavedlg.h newtoolbarstuff.h

SUBDIRS = pics
