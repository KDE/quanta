noinst_LTLIBRARIES = libdialogs.la

libdialogs_la_SOURCES = dtepeditdlg.cpp dtepeditdlgs.ui \
	dtepstructureeditdlgs.ui donationdialog.ui dtdselectdialog.ui autocompletionconfig.cpp \
	environmentconfig.cpp autocompletionconfigbase.ui environmentconfigbase.ui

METASOURCES = AUTO

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(top_srcdir)/quantacore \
		-I$(top_srcdir)/quantacore/parsers \
		-I$(top_builddir)/lib \
		$(all_includes)


noinst_HEADERS = autocompletionconfig.h environmentconfig.h
