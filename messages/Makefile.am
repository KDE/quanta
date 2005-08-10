noinst_LTLIBRARIES = libmessages.la

libmessages_la_SOURCES = messageoutput.cpp messageitem.cpp annotationoutput.cpp
METASOURCES = AUTO

AM_CPPFLAGS = -I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/lib \
	$(all_includes)

noinst_HEADERS = annotationoutput.h
