kde4_header()


kde4_footer()



#original Makefile.am contents follow:

#noinst_LTLIBRARIES = libdtdparser.la
#libdtdparser_la_SOURCES = dtepcreationdlg.ui dtdparser.cpp
#
#METASOURCES = AUTO
#
#AM_CPPFLAGS = -I$(top_srcdir)/quantacore/parsers \
#	-I$(top_srcdir)/quantacore/dialogs \
#	-I$(top_srcdir)/lib \
#	-I$(top_srcdir)/quantacore \
#	-I$(top_builddir)/quantacore/dialogs \
#	$(LIBXML_CFLAGS) $(all_includes)
#libdtdparser_la_LIBADD = $(LIBXML_LIBS)
