noinst_LTLIBRARIES = libutility.la

METASOURCES = AUTO

libutility_la_SOURCES = quantacommon.cpp tagaction.cpp toolbartabwidget.cpp toolbarxmlgui.cpp

AM_CPPFLAGS = -I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/messages \
	-I$(top_srcdir)/quanta/dialogs/tagdialogs \
	-I$(top_srcdir)/quanta/parts/kafka \
	-I$(top_srcdir)/lib \
	$(all_includes)
