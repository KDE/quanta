METASOURCES = AUTO

kde_module_LTLIBRARIES = libquantadebuggergubed.la

libquantadebuggergubed_la_SOURCES = quantadebuggergubed.cpp
libquantadebuggergubed_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
libquantadebuggergubed_la_LIBADD = ../interfaces/libdebuggerinterface.la $(LIB_KPARTS) 
kde_services_DATA =  quantadebuggergubed.desktop

INCLUDES =  -I$(top_srcdir)/quanta/components/debugger \
            -I$(top_srcdir)/quanta/components/debugger/interfaces \
            -I$(top_srcdir)/quanta/components/debugger/gubed \
            -I$(top_srcdir)/quanta/project \
            -I$(top_srcdir)/quanta/utility \
            $(all_includes)
