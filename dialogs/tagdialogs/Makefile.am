noinst_LTLIBRARIES = libtagdialogs.la

libtagdialogs_la_SOURCES = tagmisc.ui listdlg.cpp tagmail.ui tagmaildlg.cpp tagquicklistdlg.cpp tagquicklistdlgdata.cpp tagdialog.cpp tagattr.cpp pictureview.cpp tagimgdlg.cpp tagxml.cpp tagwidget.cpp colorcombo.cpp tagimgdlgdata.cpp

AM_CPPFLAGS = -I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/dialogs \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/parts/kafka \
	-I$(top_srcdir)/lib \
	$(KMDI_INCLUDES) $(all_includes)
        
METASOURCES = AUTO

