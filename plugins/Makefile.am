noinst_LTLIBRARIES = libplugins.la

AM_CPPFLAGS = -I$(top_srcdir)/quanta -I$(top_srcdir)/quanta/parser -I$(top_srcdir)/quanta/messages -I$(top_srcdir)/quanta/project $(all_includes)

METASOURCES = AUTO

libplugins_la_SOURCES = spellchecker.cpp quantaplugininterface.cpp quantaplugineditor.cpp quantapluginconfig.cpp quantaplugin.cpp plugineditor.ui pluginconfig.ui

SUBDIRS = php3dbg php4dbg framewizard csseditor

