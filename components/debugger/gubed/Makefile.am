METASOURCES = AUTO

kde_module_LTLIBRARIES = quantadebuggergubed.la

quantadebuggergubed_la_SOURCES = gubedsettingss.ui quantadebuggergubed.cpp  gubedsettings.cpp
quantadebuggergubed_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
quantadebuggergubed_la_LIBADD = ../interfaces/libdebuggerinterface.la $(LIB_KPARTS)
kde_services_DATA =  quantadebuggergubed.desktop

INCLUDES =  -I$(top_srcdir)/quanta/components/debugger \
            -I$(top_srcdir)/quanta/components/debugger/interfaces \
            -I$(top_srcdir)/quanta/components/debugger/gubed \
            -I$(top_srcdir)/quanta/project \
            -I$(top_srcdir)/quanta/utility \
            $(all_includes)


noinst_HEADERS =  gubedsettings.h
