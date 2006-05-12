
add_subdirectory( dtd )

include_directories( 
   ${KDE4_INCLUDE_DIR} 
   ${QT_INCLUDES}  
   )


########### next target ###############


########### install files ###############





#original Makefile.am contents follow:

#SUBDIRS = dtd
#METASOURCES = AUTO
#
#noinst_LTLIBRARIES = libparser.la
#libparser_la_SOURCES = parser.cpp saparser.cpp parsercommon.cpp \
#		sagroupparser.cpp parsermanager.cpp
#
#AM_CPPFLAGS = -I$(top_srcdir)/quantacore \
#	-I$(top_srcdir)/lib \
#	$(all_includes)
#
#
#noinst_HEADERS = saparser.h parsercommon.h sagroupparser.h parsermanager.h
