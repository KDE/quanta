lib_LTLIBRARIES = libkdevquanta.la
libkdevquanta_la_SOURCES = settings.cpp settingsbase.kcfgc resizablewidget.cpp \
	useraction.cpp quantacoreif.cpp myprocess.cpp node.cpp tag.cpp qtag.cpp tagattr.cpp \
	editorsource.cpp filecombo.cpp tagdialogsif.cpp basetreeview.cpp fileinfodlg.ui \
	helper.cpp quantanetaccess.cpp tagpair.cpp groupstorage.cpp extfileinfo.cpp \
	quantaprojectif.cpp searchlinewidget.cpp

AM_CPPFLAGS = $(all_includes)
METASOURCES = AUTO

noinst_HEADERS = resizablewidget.h useraction.h quantacoreif.h node.h tag.h \
	areastruct.h qtag.h tagattr.h dtdstruct.h parseresult.h editorsource.h filecombo.h \
	tagdialogsif.h basetreeview.h helper.h quantanetaccess.h groupstorage.h groupelement.h \
	tagpair.h extfileinfo.h quantaprojectif.h filterableitemif.h searchlinewidget.h \
	settings.h
libkdevquanta_la_LDFLAGS = -avoid-version
INCLUDES = -I$(top_srcdir)/lib -I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util

kde_kcfg_DATA = quanta.kcfg
