####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = dbg_core.c dbg_core.h ListenerBase.h phpdbg_idl.h ListenerGlobals.cpp ListenerBase.cpp RequestorBase.h DbgSite.h ThreadBase.h ListenerGlobals.h ThreadBase.cpp 

####### kdevelop will overwrite this part!!! (end)############

noinst_LIBRARIES = libdbgbase.a

INCLUDES = $(all_includes)
#DEFS = -I. -I$(srcdir) -I$(top_srcdir)
libdbgbase_a_METASOURCES = AUTO
libdbgbase_a_SOURCES = \
		ListenerBase.cpp\
		ListenerGlobals.cpp\
		ThreadBase.cpp\
		dbg_core.c
#DEFS = -I. -I$(srcdir) -I$(top_srcdir) -DDBG_USE_STDALLOCA -DPHP_DEBUG -D_DEBUG
DEFS = -I. -I$(srcdir) -I$(top_srcdir) -DDBG_USE_STDALLOCA -DHAVE_CONFIG_H
