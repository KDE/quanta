SUBDIRS = . pics

KDE_CXXFLAGS = $(USE_EXCEPTIONS)
noinst_LTLIBRARIES = libkafkalibrary.la

libkafkalibrary_la_SOURCES = htmlenhancer.cpp domtreeview.cpp kafkacommon.cpp \
	kafkahtmlpart.cpp nodeproperties.cpp wkafkapart.cpp undoredo.cpp kafkasyncoptionsui.ui \
	htmldocumentpropertiesui.ui kafkasyncoptions.cpp htmldocumentproperties.cpp cursors.cpp \
	kafkadragobject.cpp

libkafkalibrary_la_METASOURCES = AUTO
libkafkalibrary_la_LDFLAGS = $(all_libraries)
libkafkalibrary_la_LIBADD = $(LIB_KHTML)

kafkapartdir = $(kde_datadir)/kafkapart
kafkapart_DATA = entities

AM_CPPFLAGS = -I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/project \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/treeviews \
	-I$(top_srcdir)/lib \
	$(all_includes)
noinst_HEADERS = kafkadragobject.h
