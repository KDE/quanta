INCLUDES = -I$(top_srcdir)/lib \
	-I$(top_srcdir)/quantacore/parsers \
	-I$(top_srcdir)/quantacore/parsers/dtd \
	-I$(top_srcdir)/quantacore/dialogs \
	-I$(top_srcdir)/quantacore/autocomplete \
	-I$(top_builddir)/quantacore/dialogs \
	-I$(top_builddir)/lib \
	-I$(kde_includes)/kinterfacedesigner \
	-I$(kde_includes)/kdevelop/interfaces \
	-I$(kde_includes)/kdevelop/util \
	$(all_includes)


METASOURCES = AUTO

kde_module_LTLIBRARIES = libkdevquantacore.la
libkdevquantacore_la_SOURCES = quantacorepart.cpp dtds.cpp quantacommon.cpp \
	quantadoc.cpp
libkdevquantacore_la_LDFLAGS = -L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
libkdevquantacore_la_LIBADD = $(top_builddir)/quantacore/dialogs/libdialogs.la \
	$(top_builddir)/quantacore/parsers/libparser.la $(top_builddir)/quantacore/parsers/dtd/libdtdparser.la \
	$(top_builddir)/quantacore/autocomplete/libautocomplete.la -lkdevquanta -lkdevelop

servicedir = $(kde_servicesdir)
service_DATA = kdevquantacore.desktop

rcdir = $(kde_datadir)/kdevquantacore
rc_DATA = kdevquantacore.rc

SUBDIRS = dialogs parsers app_templates autocomplete
noinst_HEADERS = quantadoc.h
