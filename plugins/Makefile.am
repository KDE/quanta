noinst_LTLIBRARIES = libplugins.la

METASOURCES = AUTO

libplugins_la_SOURCES = quantaplugininterface.cpp quantaplugineditor.cpp quantapluginconfig.cpp quantaplugin.cpp plugineditor.ui pluginconfig.ui

AM_CPPFLAGS = -I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/quanta/messages \
	-I$(top_srcdir)/lib \
	$(KMDI_INCLUDES) $(all_includes)


