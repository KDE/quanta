####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libkxsldbgpart.la


## INCLUDES were found outside kdevelop specific part

libkxsldbgpart_la_METASOURCES = AUTO

libkxsldbgpart_la_SOURCES = xsldbgwalkspeedimpl.cpp xsldbgtemplatesimpl.cpp xsldbgtemplatelistitem.cpp xsldbgsourcesimpl.cpp xsldbgoutputview.cpp xsldbglocalvariablesimpl.cpp xsldbglocallistitem.cpp xsldbglistitem.cpp xsldbginspector.cpp xsldbgglobalvariablesimpl.cpp xsldbggloballistitem.cpp xsldbgentitiesimpl.cpp xsldbgdoc.cpp xsldbgdialogbase.cpp xsldbgdebugger.cpp xsldbgconfigimpl.cpp xsldbgcallstackimpl.cpp xsldbgbreakpointsimpl.cpp xsldbgbreakpointlistitem.cpp qxsldbgview.cpp qxsldbgdoc.cpp kxsldbg_part.cpp xsldbgwalkspeed.ui xsldbgtemplates.ui xsldbgsources.ui xsldbgmsgdialog.ui xsldbglocalvariables.ui xsldbgglobalvariables.ui xsldbgentities.ui xsldbgconfig.ui xsldbgcallstack.ui xsldbgbreakpoints.ui 

SUBDIRS = libqtnotfier libxsldbg 

EXTRA_DIST = xsldbgbreakpoints.ui xsldbgcallstack.ui xsldbgconfig.ui xsldbgentities.ui xsldbgglobalvariables.ui xsldbglocalvariables.ui xsldbgmsgdialog.ui xsldbgsources.ui xsldbgtemplates.ui xsldbgwalkspeed.ui kxsldbg_part.cpp qxsldbgdoc.cpp qxsldbgview.cpp xsldbgbreakpointlistitem.cpp xsldbgbreakpointsimpl.cpp xsldbgcallstackimpl.cpp xsldbgconfigimpl.cpp xsldbgdebugger.cpp xsldbgdialogbase.cpp xsldbgdoc.cpp xsldbgentitiesimpl.cpp xsldbggloballistitem.cpp xsldbgglobalvariablesimpl.cpp xsldbginspector.cpp xsldbglistitem.cpp xsldbglocallistitem.cpp xsldbglocalvariablesimpl.cpp xsldbgoutputview.cpp xsldbgsourcesimpl.cpp xsldbgtemplatelistitem.cpp xsldbgtemplatesimpl.cpp xsldbgwalkspeedimpl.cpp kxsldbg_part.h qxsldbgdoc.h qxsldbgview.h xsldbgbreakpointlistitem.h xsldbgbreakpointsimpl.h xsldbgcallstackimpl.h xsldbgconfigimpl.h xsldbgdebugger.h xsldbgdialogbase.h xsldbgdoc.h xsldbgdocmap.h xsldbgentitiesimpl.h xsldbggloballistitem.h xsldbgglobalvariablesimpl.h xsldbginspector.h xsldbglistitem.h xsldbglocallistitem.h xsldbglocalvariablesimpl.h xsldbgmsgdialogimpl.h xsldbgoutputview.h xsldbgsourcesimpl.h xsldbgtemplatelistitem.h xsldbgtemplatesimpl.h xsldbgwalkspeedimpl.h kxsldbg_part.desktop kxsldbg_part.rc hi16-action-1downarrow.png hi16-action-configure.png hi16-action-exit.png hi16-action-hash.png hi16-action-mark.png hi16-action-next.png hi16-action-run.png hi16-action-step.png hi16-action-xsldbg_break.png hi16-action-xsldbg_data.png hi16-action-xsldbg_delete.png hi16-action-xsldbg_enable.png hi16-action-xsldbg_output.png hi16-action-xsldbg_refresh.png hi16-action-xsldbg_source.png hi16-action-xsldbg_stepdown.png hi16-action-xsldbg_stepup.png hi22-action-1downarrow.png hi22-action-configure.png hi22-action-exit.png hi22-action-next.png hi22-action-run.png hi22-action-step.png hi22-action-xsldbg_break.png hi22-action-xsldbg_data.png hi22-action-xsldbg_delete.png hi22-action-xsldbg_enable.png hi22-action-xsldbg_output.png hi22-action-xsldbg_refresh.png hi22-action-xsldbg_source.png hi22-action-xsldbg_stepdown.png hi22-action-xsldbg_stepup.png 

####### kdevelop will overwrite this part!!! (end)############

INCLUDES = $(XSLDBG_CFLAGS) -I.. $(all_includes)


libkxsldbgpart_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
libkxsldbgpart_la_LIBADD  =  libxsldbg/libxsldbg.la libqtnotfier/libqtnotfier.la $(LIB_KPARTS) 
        

# this is where the desktop file will go
partdesktopdir   = $(kde_servicesdir)
partdesktop_DATA = kxsldbg_part.desktop

# this is where the part's XML-GUI resource file goes
partrcdir   = $(kde_datadir)/kxsldbgpart
partrc_DATA = kxsldbg_part.rc

KDE_ICON = AUTO

