####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = highlight 

EXTRA_DIST = kwbuffer.h kwdialog.cpp kwrite_factory.h undohistory.cpp kwdoc.cpp kwdoc.h kwview.cpp ktextprint.cpp kwriteIface_skel.cpp kwdialog.h kwbuffer.cpp kwview.h kvmallocator.h ktextprint.h kwtextline.h kwattribute.cpp kwattribute.h kwrite_factory.cpp kwriteIface.h kwtextline.cpp undohistory.h kvmallocator.cpp 

####### kdevelop will overwrite this part!!! (end)############
noinst_LIBRARIES = libkwrite.a



INCLUDES = $(all_includes)\
		-I$(top_srcdir)/quanta/kwrite/highlight
DEFS = -I. -I$(srcdir) -I$(top_srcdir)
libkwrite_a_METASOURCES = AUTO
libkwrite_a_SOURCES = \
		kwriteIface_skel.cpp\
		ktextprint.cpp\
		kvmallocator.cpp\
		kwattribute.cpp\
		kwbuffer.cpp\
		kwdialog.cpp\
		kwdoc.cpp\
		kwrite_factory.cpp\
		kwtextline.cpp\
		kwview.cpp\
		undohistory.cpp
