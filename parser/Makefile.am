noinst_LTLIBRARIES = libparser.la

AM_CPPFLAGS = -I$(top_srcdir)/quanta/ \
	-I$(top_srcdir)/quanta/parts/kafka \
	-I$(top_srcdir)/quanta/treeviews \
	$(LIBXML_CFLAGS) $(all_includes)


METASOURCES = AUTO

libparser_la_SOURCES = qtag.cpp node.cpp tag.cpp parser.cpp saparser.cpp parsercommon.cpp dtdparser.cpp dtepcreationdlg.ui
