noinst_LTLIBRARIES = libtableeditor.la

libtableeditor_la_SOURCES = tableeditors.ui tableeditor.cpp tableitem.cpp

METASOURCES = AUTO

AM_CPPFLAGS = -I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/dialogs/tagdialogs \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/quanta/components/tableeditor \
	$(KMDI_INCLUDES) $(all_includes)

noinst_HEADERS =  tableeditor.h tableitem.h
