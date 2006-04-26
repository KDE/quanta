kde4_header()

include_directories(
 ${CMAKE_SOURCE_DIR}/lib
 ${CMAKE_BINARY_DIR}/lib
 ${CMAKE_BINARY_DIR}/plugins/filestree
 ${KDE4_INCLUDE_DIR}/kdevelop/interfaces
 ${KDE4_INCLUDE_DIR}/kdevelop/util
 ${KDE4_INCLUDES}
)


########### next target ###############

set(kdevquantafilestree_PART_SRCS 
   quantafilestreepart.cpp 
   quantafilestreeglobalconfig.cpp 
   quantafilestreeprojectconfig.cpp 
   filestreeview.cpp )

kde4_automoc(${kdevquantafilestree_PART_SRCS})

kde4_add_ui3_files(kdevquantafilestree_PART_SRCS quantafilestreeglobalconfigbase.ui quantafilestreeprojectconfigbase.ui )

kde4_add_plugin(kdevquantafilestree WITH_PREFIX ${kdevquantafilestree_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevquantafilestree )

target_link_libraries(kdevquantafilestree  kdevquanta kdevinterfaces kdevutil ${KDE4_KDECORE_LIBS} )

install_targets(${LIB_INSTALL_DIR}/kde4 kdevquantafilestree )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kdevquantafilestree FILES  )
install_files( ${SERVICES_INSTALL_DIR} FILES  kdevquantafilestree.desktop )
install_files( ${DATA_INSTALL_DIR}/kdevquantafilestree FILES  kdevquantafilestree.rc )

kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
#	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)
#
#kde_module_LTLIBRARIES = libkdevquantafilestree.la
#libkdevquantafilestree_la_LDFLAGS = -avoid-version -module -no-undefined \
#	-L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
#libkdevquantafilestree_la_LIBADD = -lkdevquanta -lkdevelop
#libkdevquantafilestree_la_SOURCES = quantafilestreepart.cpp \
#	quantafilestreeglobalconfigbase.ui quantafilestreeglobalconfig.cpp quantafilestreeprojectconfigbase.ui \
#	quantafilestreeprojectconfig.cpp filestreeview.cpp
#
#METASOURCES = AUTO
#
#servicedir = $(kde_servicesdir)
#service_DATA = kdevquantafilestree.desktop
#
#rcdir = $(kde_datadir)/kdevquantafilestree
#rc_DATA = kdevquantafilestree.rc
#
#appsharedir = $(kde_datadir)/kdevquantafilestree
#appshare_DATA =
#noinst_HEADERS = filestreeview.h
