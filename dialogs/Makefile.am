noinst_LTLIBRARIES = libdialogs.la

libdialogs_la_SOURCES = copyto.cpp dirtydialog.ui dirtydlg.cpp donationdialog.ui dtdselectdialog.ui kategrepdialog.cpp fourbuttonmessagebox.ui casewidget.ui actionconfigdialogs.ui actionconfigdialog.cpp filecombo.cpp

METASOURCES = AUTO

AM_CPPFLAGS = -I$(top_srcdir)/lib \
	-I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/src \
	$(KMDI_INCLUDES) $(all_includes)

SUBDIRS = settings tagdialogs
