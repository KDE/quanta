SUBDIRS = dtd
METASOURCES = AUTO

noinst_LTLIBRARIES = libparser.la
libparser_la_SOURCES = qtag.cpp node.cpp tag.cpp parser.cpp saparser.cpp \
	parsercommon.cpp sagroupparser.cpp

AM_CPPFLAGS = -I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/parts/kafka \
	-I$(top_srcdir)/quanta/treeviews \
	-I$(top_srcdir)/lib \
	$(KMDI_INCLUDES) $(all_includes)


noinst_HEADERS = saparser.h parsercommon.h sagroupparser.h
