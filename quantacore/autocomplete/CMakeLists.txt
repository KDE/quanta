kde4_header()

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR} 
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_SOURCE_DIR}/quanta/lib 
   ${CMAKE_SOURCE_DIR}/quanta/quantacore 
   ${CMAKE_SOURCE_DIR}/quanta/quantacore/parsers 
   ${KDE4_INCLUDE_DIR} 
   ${QT_INCLUDES}  
   )


########### next target ###############


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#noinst_HEADERS = completionbase.h xmltagcompletion.h xmlattributecompletion.h \
#	xmlattributevaluecompletion.h dtdcompletion.h xmlentitycompletion.h specialareagroupcompletion.h \
#	specialareatagcompletion.h specialareaattributecompletion.h
#libautocomplete_la_LDFLAGS = $(all_libraries)
#noinst_LTLIBRARIES = libautocomplete.la
#libautocomplete_la_SOURCES = completionbase.cpp xmltagcompletion.cpp \
#	xmlattributecompletion.cpp xmlattributevaluecompletion.cpp dtdcompletion.cpp \
#	xmlentitycompletion.cpp specialareagroupcompletion.cpp specialareatagcompletion.cpp \
#	specialareaattributecompletion.cpp
#INCLUDES = -I$(top_srcdir)/lib -I$(top_srcdir)/quantacore \
#	-I$(top_srcdir)/quantacore/parsers -I$(kde_includes)/kdevelop/util -I$(kde_includes)/kdevelop/interfaces \
#	-I$(top_builddir)/lib $(all_includes)
