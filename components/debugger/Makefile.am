SUBDIRS = interfaces gubed 
METASOURCES = AUTO


noinst_LTLIBRARIES =  libdebuggermanager.la

libdebuggermanager_la_LDFLAGS = $(all_libraries)
libdebuggermanager_la_LIBADD = interfaces/libdebuggerinterface.la
libdebuggermanager_la_SOURCES =  debuggermanager.cpp debuggerbreakpoint.cpp quantadebuggerinterface.cpp  debuggerbreakpointlist.cpp
noinst_HEADERS = debuggermanager.h debuggerbreakpoint.h debuggerbreakpointlist.h

AM_CPPFLAGS =  -I$(top_srcdir)/quanta/components/debuggerquanta \
	-I$(top_srcdir)/quanta/components/debugger/interfaces  \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/messages \
	-I$(top_srcdir)/lib \
	$(KMDI_INCLUDES) $(all_includes)
