noinst_LTLIBRARIES = libutility.la

METASOURCES = AUTO

libutility_la_SOURCES = quantacommon.cpp tagaction.cpp toolbartabwidget.cpp \
			toolbarxmlgui.cpp newstuff.cpp quantanetaccess.cpp qpevents.cpp quantabookmarks.cpp

AM_CPPFLAGS = -I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/messages \
	-I$(top_srcdir)/quanta/dialogs/tagdialogs \
	-I$(top_srcdir)/quanta/parts/kafka \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/lib \
	$(QNEWSTUFF_INCLUDES) \
	$(KMDI_INCLUDES) $(all_includes)

libutility_la_LDFLAGS = $(all_libraries)
libutility_la_LIBADD = $(LIB_QNEWSTUFF)
noinst_HEADERS = qpevents.h
