
include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop
   )


########### next target ###############

set(kdevquantafilestree_PART_SRCS 
   quantafilestreepart.cpp 
   quantafilestreeglobalconfig.cpp 
   quantafilestreeprojectconfig.cpp 
   filestreeview.cpp )

kde4_automoc(${kdevquantafilestree_PART_SRCS})

kde4_add_ui_files(kdevquantafilestree_PART_SRCS
  quantafilestreeglobalconfigbase.ui
  quantafilestreeprojectconfigbase.ui )

kde4_add_plugin(kdevquantafilestree WITH_PREFIX ${kdevquantafilestree_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevquantafilestree )

target_link_libraries(kdevquantafilestree  kdevquanta kdevplatform kdevutil ${KDE4_KDECORE_LIBS} )

install(TARGETS kdevquantafilestree  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevquantafilestree.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevquantafilestree.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevquantafilestree)




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
