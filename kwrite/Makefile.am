####### kdevelop will overwrite this part!!! (begin)##########

INCLUDES = $(all_includes)

libkwrite_a_METASOURCES = AUTO

noinst_LIBRARIES = libkwrite.a

libkwrite_a_SOURCES = undohistory.cpp kwview.cpp kwrite_factory.cpp kwdoc.cpp kwdialog.cpp ktextprint.cpp 


EXTRA_DIST = ktextprint.cpp ktextprint.h kwdialog.cpp kwdialog.h kwdoc.cpp kwdoc.h kwriteIface.h kwrite_factory.cpp kwrite_factory.h kwview.cpp kwview.h undohistory.cpp undohistory.h 

####### kdevelop will overwrite this part!!! (end)############
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
