
METASOURCES = AUTO

noinst_LTLIBRARIES =  libdebuggerinterface.la
libdebuggerinterface_la_LDFLAGS = $(all_libraries)
libdebuggerinterface_la_SOURCES = debuggerinterface.cpp debuggerclient.cpp


# Definition of the service type
kde_servicetypes_DATA = quantadebugger.desktop


INCLUDES = -I$(top_srcdir)/quanta/components/debugger \
   -I$(top_srcdir)/quanta/components/debugger/interfaces  \
   -I$(top_srcdir)/quanta/project \
   -I$(top_srcdir)/utility \
   $(all_includes)
