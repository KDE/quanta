SUBDIRS = data

noinst_LTLIBRARIES = libcsseditor.la

libcsseditor_la_SOURCES = fontfamilychoosers.ui encodingselectors.ui cssselectors.ui csseditors.ui fontfamilychooser.cpp encodingselector.cpp cssselector.cpp csseditor.cpp specialsb.cpp doubleeditors.cpp qmyhighlighter.cpp colorrequester.cpp propertysetter.cpp shorthandformer.cpp cssshpropertyparser.cpp
libcsseditor_la_METASOURCES = AUTO

AM_CPPFLAGS = -I$(top_srcdir)/quanta/components/framewizard \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/lib \
	$(all_includes)
