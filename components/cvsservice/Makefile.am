noinst_LTLIBRARIES = libcvsservice.la

libcvsservice_la_SOURCES = cvsservice.cpp cvscommitdlgs.ui \
	cvsservicedcopif.skel cvsupdatetodlgs.ui
libcvsservice_la_METASOURCES = AUTO

AM_CPPFLAGS = -I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/lib \
	$(KMDI_INCLUDES) $(all_includes)
noinst_HEADERS =  cvsservicedcopif.h
