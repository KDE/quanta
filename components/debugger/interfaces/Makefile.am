
METASOURCES = AUTO

noinst_LTLIBRARIES =  libdebuggerinterface.la
libdebuggerinterface_la_LDFLAGS = $(all_libraries)  -version-info 0:0:0
libdebuggerinterface_la_SOURCES = debuggerinterface.cpp debuggerclient.cpp

AM_CPPFLAGS = -I$(top_srcdir)/quanta/components/debugger \
	-I$(top_srcdir)/quanta/components/debugger/interfaces  \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/utility \
	$(all_includes)
