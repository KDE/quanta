noinst_LTLIBRARIES = libphpdebug.la

libphpdebug_la_SOURCES = phpdebuggerinterface.cpp phpdebugsocket.cpp phpdebuggubed.cpp 
libphpdebug_la_METASOURCES = AUTO

AM_CPPFLAGS = -I$(top_srcdir)/quanta/src \
        -I$(top_srcdir)/quanta/utility \
        -I$(top_srcdir)/quanta/components/phpdebug \
        -I$(top_srcdir)/quanta/parsers \
        -I$(top_srcdir)/quanta/project \
        -I$(top_srcdir)/quanta/messages \
        -I$(top_srcdir)/lib \
        $(all_includes)
