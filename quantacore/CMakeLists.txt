kde4_header()
find_package(LibXml2 REQUIRED)

add_subdirectory( dialogs )
add_subdirectory( parsers )
add_subdirectory( app_templates )
add_subdirectory( autocomplete )

include_directories( ${LIBXML2_INCLUDE_DIR} )

include_directories( 
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_SOURCE_DIR}/quanta/quantacore/parsers
   ${CMAKE_SOURCE_DIR}/quanta/quantacore/parsers/dtd
   ${CMAKE_SOURCE_DIR}/quanta/quantacore/dialogs
   ${CMAKE_SOURCE_DIR}/quanta/quantacore/autocomplete
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop
   )


########### next target ###############

set(kdevquantacore_PART_SRCS 
   quantacorepart.cpp 
   dtds.cpp 
   quantacommon.cpp 
   quantadoc.cpp
   dialogs/autocompletionconfig.cpp
   dialogs/dtepeditdlg.cpp
   dialogs/environmentconfig.cpp

   parsers/comparator.cpp
   parsers/parsercommon.cpp
   parsers/parser.cpp
   parsers/parsermanager.cpp
   parsers/sagroupparser.cpp
   parsers/saparser.cpp

   parsers/dtd/dtdparser.cpp

   autocomplete/completionbase.cpp
   autocomplete/dtdcompletion.cpp
   autocomplete/specialareaattributecompletion.cpp
   autocomplete/specialareagroupcompletion.cpp
   autocomplete/specialareatagcompletion.cpp
   autocomplete/xmlattributecompletion.cpp
   autocomplete/xmlattributevaluecompletion.cpp
   autocomplete/xmlentitycompletion.cpp
   autocomplete/xmltagcompletion.cpp
)

kde4_add_ui_files(kdevquantacore_PART_SRCS
dialogs/autocompletionconfigbase.ui
dialogs/dtepeditdlgs.ui
dialogs/dtepstructureeditdlgs.ui
dialogs/dtdselectdialog.ui
dialogs/donationdialog.ui
dialogs/environmentconfigbase.ui

parsers/dtd/dtepcreationdlg.ui
)   

kde4_automoc(${kdevquantacore_PART_SRCS})

kde4_add_plugin(kdevquantacore WITH_PREFIX ${kdevquantacore_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevquantacore )

target_link_libraries(kdevquantacore kdevquanta
kdevinterfaces kdevutil ktexteditor
${LIBXML2_LIBRARIES} ${KDE4_KDECORE_LIBS})

install_targets(${LIB_INSTALL_DIR}/kde4 kdevquantacore )


########### install files ###############

install_files( ${SERVICES_INSTALL_DIR} FILES  kdevquantacore.desktop )
install_files( ${DATA_INSTALL_DIR}/kdevquantacore FILES  kdevquantacore.rc )

kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/lib \
#	-I$(top_srcdir)/quantacore/parsers \
#	-I$(top_srcdir)/quantacore/parsers/dtd \
#	-I$(top_srcdir)/quantacore/dialogs \
#	-I$(top_srcdir)/quantacore/autocomplete \
#	-I$(top_builddir)/quantacore/dialogs \
#	-I$(top_builddir)/lib \
#	-I$(kde_includes)/kinterfacedesigner \
#	-I$(kde_includes)/kdevelop/interfaces \
#	-I$(kde_includes)/kdevelop/util \
#	$(all_includes)
#
#
#METASOURCES = AUTO
#
#kde_module_LTLIBRARIES = libkdevquantacore.la
#libkdevquantacore_la_SOURCES = quantacorepart.cpp dtds.cpp quantacommon.cpp \
#	quantadoc.cpp
#libkdevquantacore_la_LDFLAGS = -L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
#libkdevquantacore_la_LIBADD = $(top_builddir)/quantacore/dialogs/libdialogs.la \
#	$(top_builddir)/quantacore/parsers/libparser.la $(top_builddir)/quantacore/parsers/dtd/libdtdparser.la \
#	$(top_builddir)/quantacore/autocomplete/libautocomplete.la -lkdevquanta -lkdevelop
#
#servicedir = $(kde_servicesdir)
#service_DATA = kdevquantacore.desktop
#
#rcdir = $(kde_datadir)/kdevquantacore
#rc_DATA = kdevquantacore.rc
#
#SUBDIRS = dialogs parsers app_templates autocomplete
#noinst_HEADERS = quantadoc.h
