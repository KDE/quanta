kde_module_LTLIBRARIES = libkxsldbgpart.la

libkxsldbgpart_la_METASOURCES = AUTO

libkxsldbgpart_la_SOURCES = xsldbgwalkspeedimpl.cpp xsldbgtemplatesimpl.cpp xsldbgtemplatelistitem.cpp xsldbgsourcesimpl.cpp xsldbgoutputview.cpp xsldbglocalvariablesimpl.cpp xsldbglocallistitem.cpp xsldbglistitem.cpp xsldbginspector.cpp xsldbgglobalvariablesimpl.cpp xsldbggloballistitem.cpp xsldbgentitiesimpl.cpp xsldbgdoc.cpp xsldbgdialogbase.cpp xsldbgdebugger.cpp xsldbgconfigimpl.cpp xsldbgcallstackimpl.cpp xsldbgbreakpointsimpl.cpp xsldbgbreakpointlistitem.cpp qxsldbgview.cpp qxsldbgdoc.cpp kxsldbg_part.cpp xsldbgwalkspeed.ui xsldbgtemplates.ui xsldbgsources.ui xsldbgmsgdialog.ui xsldbglocalvariables.ui xsldbgglobalvariables.ui xsldbgentities.ui xsldbgconfig.ui xsldbgcallstack.ui xsldbgbreakpoints.ui

SUBDIRS = libqtnotfier libxsldbg

INCLUDES = $(XSLDBG_CFLAGS) -I.. $(all_includes)


libkxsldbgpart_la_LDFLAGS = -module -no-undefined $(all_libraries) -version-info 1:0:0
libkxsldbgpart_la_LIBADD  =  libxsldbg/libxsldbg.la libqtnotfier/libqtnotfier.la $(LIB_KPARTS)


# this is where the desktop file will go
partdesktopdir   = $(kde_servicesdir)
partdesktop_DATA = kxsldbg_part.desktop

# this is where the part's XML-GUI resource file goes
partrcdir   = $(kde_datadir)/kxsldbgpart
partrc_DATA = kxsldbg_part.rc

KDE_ICON = AUTO

