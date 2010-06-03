# Here resides the qextmdi library.

INCLUDES = -I$(top_srcdir)/quanta/kmdi/qextmdi/res -I$(top_srcdir)/quanta/kmdi/structure $(all_includes)

lib_LTLIBRARIES = libquantaqextmdi.la
libquantaqextmdi_la_LDFLAGS = $(all_libraries) -version-info 3:0
libquantaqextmdi_la_LIBADD = $(LIB_KPARTS)

libquantaqextmdi_la_SOURCES = kmdichildarea.cpp kmdichildfrm.cpp kmdichildfrmcaption.cpp \
				kmdichildview.cpp kmdimainfrm.cpp kmditaskbar.cpp kmdidockcontainer.cpp \
				kdockwidget_compat.cpp dockmainwindow_compat.cpp kmultitabbar.cpp \
				kmditoolviewaccessor.cpp kmdidocumentviewtabwidget.cpp kmdifocuslist.cpp \
				kmdiguiclient.cpp \
				ktabwidget.cpp ktabbar.cpp

KDE_OPTIONS = nofinal
METASOURCES = AUTO

#AM_CXXFLAGS = -DNO_INCLUDE_MOCFILES
