####### kdevelop will overwrite this part!!! (begin)##########

EXTRA_DIST = dbg_core.c dbg_core.h ListenerBase.h phpdbg_idl.h ListenerGlobals.cpp ListenerBase.cpp RequestorBase.h DbgSite.h ThreadBase.h ListenerGlobals.h ThreadBase.cpp 

####### kdevelop will overwrite this part!!! (end)############

noinst_LTLIBRARIES = libdbgbase.la

INCLUDES = -DDBG_USE_STDALLOCA $(all_includes)
METASOURCES = AUTO
libdbgbase_la_SOURCES = \
		ListenerBase.cpp\
		ListenerGlobals.cpp\
		ThreadBase.cpp\
		dbg_core.c
