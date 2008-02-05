noinst_LTLIBRARIES = libtreeviews.la

METASOURCES = AUTO

libtreeviews_la_SOURCES = uploadtreeview.cpp uploadtreefolder.cpp uploadtreefile.cpp fileinfodlg.ui quantapropertiespage.ui newtemplatedirdlg.cpp templatedirform.ui templatestreeview.cpp projecttreeview.cpp structtreeview.cpp doctreeview.cpp docitem.cpp docfolder.cpp structtreetag.cpp filestreeview.cpp tagattributetree.cpp tagattributeitems.cpp scripttreeview.cpp basetreeview.cpp servertreeview.cpp

if include_cvsservice
CVSSERVICE_INCLUDE = -I$(top_srcdir)/quanta/components/cvsservice
endif

AM_CPPFLAGS = -I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/quanta/dialogs/tagdialogs \
	-I$(top_srcdir)/quanta/dialogs \
	-I$(top_srcdir)/quanta/messages \
	-I$(top_srcdir)/quanta/parts/kafka \
	-I$(top_srcdir)/quanta/components/csseditor \
	-I$(top_srcdir)/lib \
	-I$(top_builddir)/quanta/dialogs/tagdialogs \
	-I$(top_builddir)/quanta/components/csseditor \
	-I$(top_srcdir)/quanta/plugins \
	$(KMDI_INCLUDES) $(CVSSERVICE_INCLUDE) $(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS) $(all_includes)
