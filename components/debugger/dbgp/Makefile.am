METASOURCES = AUTO

kde_module_LTLIBRARIES = quantadebuggerdbgp.la

quantadebuggerdbgp_la_SOURCES = quantadebuggerdbgp.cpp dbgpsettingss.ui \
	dbgpsettings.cpp dbgpnetwork.cpp qbytearrayfifo.cpp
quantadebuggerdbgp_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
quantadebuggerdbgp_la_LIBADD = ../interfaces/libdebuggerinterface.la $(LIB_KPARTS)
kde_services_DATA =  quantadebuggerdbgp.desktop

INCLUDES = -I$(top_srcdir)/quanta/components/debugger \
	-I$(top_srcdir)/quanta/components/debugger/interfaces -I$(top_srcdir)/quanta/components/debugger/dbgp \
	-I$(top_srcdir)/quanta/project -I$(top_srcdir)/quanta/utility $(all_includes)


noinst_HEADERS = dbgpsettings.h quantadebuggerdbgp.h dbgpnetwork.h \
	qbytearrayfifo.h
