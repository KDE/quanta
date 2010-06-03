# Here resides the library of general KDevelop-related utilities,
# which may be linked by any plugin or part. This is installed
# as a shared library, including header files


INCLUDES = $(all_includes)

lib_LTLIBRARIES = libquantakmdi.la
libquantakmdi_la_LDFLAGS = $(all_libraries) -no-undefined
libquantakmdi_la_LIBADD = $(top_builddir)/quanta/kmdi/qextmdi/src/libqextmdi.la -lktexteditor $(LIB_KPARTS) $(LIB_KIO)
libquantakmdi_la_SOURCES = dummy.cpp


SUBDIRS = structure qextmdi

AM_CFLAGS = 
AM_CXXFLAGS = 
AM_FFLAGS = 
#lib_LIBRARIES =  libquanta_la
#libquanta_la_SOURCES = 
#libquanta_la_LDFLAGS = 
