INCLUDES = $(all_includes)

libkwrite_a_METASOURCES = AUTO

KDE_CXXFLAGS = $(USE_RTTI)

noinst_LIBRARIES = libkwrite.a

libkwrite_a_SOURCES = ktextprint.cpp  \
	kwdoc.cpp kwview.cpp undohistory.cpp  \
	kwdialog.cpp kwrite_factory.cpp kwriteIface.skel

EXTRA_DIST = ktextprint.cpp kwdoc.cpp kwview.cpp \
	kwdialog.cpp kwrite_factory.cpp undohistory.cpp \
	ktextprint.h kwdoc.h kwrite_factory.h undohistory.h \
	kwdialog.h kwriteIface.h kwview.h

SUBDIRS = highlight
