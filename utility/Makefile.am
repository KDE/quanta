noinst_LTLIBRARIES = libutility.la

METASOURCES = AUTO

libutility_la_SOURCES = quantacommon.cpp tagaction.cpp toolbartabwidget.cpp \
				toolbarxmlgui.cpp newstuff.cpp quantanetaccess.cpp qpevents.cpp quantabookmarks.cpp \
	tagactionmanager.cpp tagactionset.cpp

AM_CPPFLAGS = -I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/messages \
	-I$(top_srcdir)/quanta/dialogs/tagdialogs \
	-I$(top_srcdir)/quanta/parts/kafka \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/lib \
	$(KNEWSTUFF_INCLUDES) \
	$(KMDI_INCLUDES) $(all_includes)

libutility_la_LDFLAGS = $(all_libraries)
noinst_HEADERS = qpevents.h tagactionmanager.h tagactionset.h
