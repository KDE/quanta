noinst_LTLIBRARIES = libtableeditor.la

tableeditor.o: tableeditor.ui.h

libtableeditor_la_SOURCES = tableeditor.ui dummy.cpp tableitem.cpp

METASOURCES = AUTO

AM_CPPFLAGS = -I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/dialogs/tagdialogs \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/lib \
	$(KMDI_INCLUDES) $(all_includes)

