SUBDIRS = interfaces gubed dbgp
METASOURCES = AUTO


noinst_LTLIBRARIES =  libdebuggermanager.la

libdebuggermanager_la_LDFLAGS = $(all_libraries)
libdebuggermanager_la_LIBADD = interfaces/libdebuggerinterface.la
libdebuggermanager_la_SOURCES = debuggermanager.cpp debuggerbreakpoint.cpp \
		quantadebuggerinterface.cpp debuggervariable.cpp debuggerui.cpp variableslistview.cpp \
		debuggerbreakpointlist.cpp debuggervariablesets.ui debuggerbreakpointview.cpp pathmapper.cpp \
		pathmapperdialogs.ui pathmapperdialog.cpp pathmapperdialog.h conditionalbreakpointdialogs.ui \
		conditionalbreakpointdialog.cpp conditionalbreakpointdialog.h backtracelistview.cpp

AM_CPPFLAGS =  -I$(top_srcdir)/quanta/components/debuggerquanta \
	-I$(top_srcdir)/quanta/components/debugger/interfaces  \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/parts/preview \
	-I$(top_srcdir)/quanta/messages \
	-I$(top_srcdir)/lib \
	$(KMDI_INCLUDES) $(all_includes)
noinst_HEADERS = pathmapper.h backtracelistview.h
