noinst_LTLIBRARIES = libdtdparser.la
libdtdparser_la_SOURCES = dtepcreationdlg.ui dtdparser.cpp

METASOURCES = AUTO

AM_CPPFLAGS = -I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/dialogs \
	-I$(top_builddir)/quanta/dialogs \
	-I$(top_srcdir)/lib \
	$(LIBXML_CFLAGS) $(all_includes)
