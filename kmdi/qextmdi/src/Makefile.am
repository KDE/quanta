# Here resides the qextmdi library.

INCLUDES = -I$(top_srcdir)/quanta/kmdi/qextmdi/include -I$(top_srcdir)/quanta/kmdi/qextmdi/res -I$(top_srcdir)/quanta/kmdi/structure $(all_includes)

lib_LTLIBRARIES = libqextmdi.la
libqextmdi_la_LDFLAGS = $(all_libraries) -version-info 3:0
libqextmdi_la_LIBADD  = $(LIB_KPARTS)

libqextmdi_la_SOURCES = kmdichildarea.cpp kmdichildfrm.cpp kmdichildfrmcaption.cpp \
				kmdichildview.cpp kmdimainfrm.cpp kmditaskbar.cpp kmdidockcontainer.cpp \
				kdockwidget_compat.cpp kmultitabbar.cpp \
				kmditoolviewaccessor.cpp


KDE_OPTIONS = nofinal
METASOURCES = AUTO

META_INCLUDES = $(srcdir)/../include
