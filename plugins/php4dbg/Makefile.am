SUBDIRS = dbgbase
noinst_LTLIBRARIES = libphp4dbg.la

INCLUDES = -I$(top_srcdir)/quanta/plugins/php4dbg/dbgbase $(all_includes)
METASOURCES = AUTO
libphp4dbg_la_SOURCES = \
		debugger.cpp\
		listener.cpp\
		thread.cpp\
		idesite.cpp
#DEFS = -I. -I$(srcdir) -I$(top_srcdir) -DPHP_DEBUG -D_DEBUG
#DEFS = -I. -I$(srcdir) -I$(top_srcdir)
