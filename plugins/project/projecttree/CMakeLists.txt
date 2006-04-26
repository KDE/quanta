kde4_header()

include_directories(
${CMAKE_SOURCE_DIR}/lib
${CMAKE_BINARY_DIR}/lib
${CMAKE_BINARY_DIR}/plugins/project/projecttree
${KDE4_INCLUDE_DIR}/kdevelop/interfaces
${KDE4_INCLUDE_DIR}/kdevelop/util
${KDE4_INCLUDES}
)


########### next target ###############

set(kdevprojecttree_PART_SRCS 
   projecttreepart.cpp 
   projecttreewidget.cpp 
   projecttreeglobalconfig.cpp 
   projecttreeprojectconfig.cpp 
   projecttreeview.cpp )

kde4_automoc(${kdevprojecttree_PART_SRCS})

kde4_add_ui3_files(kdevprojecttree_PART_SRCS projecttreeglobalconfigbase.ui projecttreeprojectconfigbase.ui )

kde4_add_plugin(kdevprojecttree WITH_PREFIX ${kdevprojecttree_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevprojecttree )

target_link_libraries(kdevprojecttree  kdevquanta kdevinterfaces kdevutil ${KDE4_KDECORE_LIBS} )

install_targets(${LIB_INSTALL_DIR}/kde4 kdevprojecttree )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kdevprojecttree FILES  )
install_files( ${SERVICES_INSTALL_DIR} FILES  kdevprojecttree.desktop )
install_files( ${DATA_INSTALL_DIR}/kdevprojecttree FILES  kdevprojecttree.rc )

kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
#	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)
#
#kde_module_LTLIBRARIES = libkdevprojecttree.la
#libkdevprojecttree_la_LDFLAGS = -avoid-version -module -no-undefined \
#	-L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
#libkdevprojecttree_la_LIBADD = -lkdevquanta -lkdevelop
#libkdevprojecttree_la_SOURCES = projecttreepart.cpp projecttreewidget.cpp \
#	projecttreeglobalconfigbase.ui projecttreeglobalconfig.cpp projecttreeprojectconfigbase.ui \
#	projecttreeprojectconfig.cpp projecttreeview.cpp
#
#METASOURCES = AUTO
#
#servicedir = $(kde_servicesdir)
#service_DATA = kdevprojecttree.desktop
#
#rcdir = $(kde_datadir)/kdevprojecttree
#rc_DATA = kdevprojecttree.rc
#
#appsharedir = $(kde_datadir)/kdevprojecttree
#appshare_DATA =
#noinst_HEADERS = projecttreeview.h
