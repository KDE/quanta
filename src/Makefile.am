if BUILD_WYSIWYG
LIB_KAFKA = $(top_builddir)/quanta/parts/kafka/libkafkalibrary.la
KAFKAINCLUDE = -I$(top_srcdir)/quanta/parts/kafka -I$(top_builddir)/quanta/parts/kafka
endif

if include_cvsservice
CVSSERVICE_INCLUDE = -I$(top_srcdir)/quanta/components/cvsservice
LIB_CVSSERVICE = $(top_builddir)/quanta/components/cvsservice/libcvsservice.la -lcvsservice
endif

bin_PROGRAMS = quanta

quanta_SOURCES = quanta_init.cpp quantaview.cpp quantadoc.cpp main.cpp document.cpp kqapp.cpp quanta.cpp dcopwindowmanagerif.skel dcopsettingsif.skel dcopquantaif.skel dcopsettings.cpp dtds.cpp dcopquanta.cpp viewmanager.cpp


quanta_LDADD = $(top_builddir)/quanta/project/libproject.la \
	$(top_builddir)/quanta/plugins/libplugins.la \
	$(top_builddir)/quanta/parsers/libparser.la \
	$(top_builddir)/quanta/parsers/dtd/libdtdparser.la \
	$(top_builddir)/quanta/treeviews/libtreeviews.la \
	$(top_builddir)/quanta/dialogs/libdialogs.la \
	$(top_builddir)/quanta/components/debugger/libdebuggermanager.la \
	$(top_builddir)/quanta/dialogs/tagdialogs/libtagdialogs.la \
	$(top_builddir)/quanta/dialogs/settings/libsettingsdialogs.la \
	$(top_builddir)/quanta/messages/libmessages.la \
	$(top_builddir)/quanta/components/framewizard/libframewizard.la \
	$(top_builddir)/quanta/components/csseditor/libcsseditor.la \
	$(top_builddir)/quanta/components/tableeditor/libtableeditor.la \
	$(top_builddir)/quanta/parts/preview/libpreview.la \
	$(top_builddir)/quanta/utility/libutility.la \
	$(top_builddir)/lib/libquantamodule.la \
	-lkatepartinterfaces $(LIB_KNEWSTUFF) $(LIB_KAFKA) $(LIB_CVSSERVICE)  \
	$(LIB_KMDI) $(LIBXML_LIBS) $(LIBXSLT_LIBS) $(LIB_KABC) $(LIB_KSPELL) $(LIB_KHTML)
#       $(top_builddir)/lib/compatibility/libcompat.la

AM_CPPFLAGS = -I$(top_srcdir)/quanta/dialogs \
	-I$(top_srcdir)/quanta/plugins \
	-I$(top_srcdir)/quanta/components/framewizard \
	-I$(top_srcdir)/quanta/components/csseditor \
	-I$(top_srcdir)/quanta/components/debugger \
	-I$(top_srcdir)/quanta/components/tableeditor \
	-I$(top_srcdir)/quanta/dialogs/tagdialogs \
	-I$(top_srcdir)/quanta/dialogs/settings \
	-I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/treeviews \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/parsers/dtd \
	-I$(top_srcdir)/quanta/parts/preview \
	-I$(top_srcdir)/quanta/parts/kafka \
	-I$(top_srcdir)/quanta/messages \
	-I$(top_srcdir)/lib \
	-I$(top_builddir)/quanta/dialogs \
	-I$(top_builddir)/quanta/dialogs/tagdialogs \
	-I$(top_builddir)/quanta/dialogs/settings \
	-I$(top_builddir)/quanta/components/csseditor \
	-I$(top_builddir)/quanta/components/framewizard \
	-I$(top_builddir)/quanta/components/tableeditor \
	-I$(top_builddir)/quanta/plugins \
	$(KAFKAINCLUDE) $(CVSSERVICE_INCLUDE) $(KMDI_INCLUDES) \
	 $(KNEWSTUFF_INCLUDES) $(all_includes)
#       -I$(top_srcdir)/lib/compatibility

quanta_METASOURCES = AUTO

# the library search path.
quanta_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# this is where the kdelnk file will go
xdg_apps_DATA = quanta.desktop

kdemimedir = $(kde_mimedir)/application
kdemime_DATA = x-webprj.desktop

