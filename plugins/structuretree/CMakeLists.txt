kde4_header()

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR} 
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop/interfaces
   ${KDE4_INCLUDE_DIR}/kdevelop/util
   ${KDE4_INCLUDES}
   )


########### next target ###############

set(kdevstructuretree_PART_SRCS 
   structuretreepart.cpp 
   structuretreewidget.cpp 
   structuretreeglobalconfig.cpp 
   structuretreeprojectconfig.cpp 
   structtreeitem.cpp 
   groupswidget.cpp 
   groupstreeitem.cpp )

kde4_automoc(${kdevstructuretree_PART_SRCS})

kde4_add_ui3_files(kdevstructuretree_PART_SRCS structuretreeglobalconfigbase.ui structuretreeprojectconfigbase.ui )

kde4_add_plugin(kdevstructuretree WITH_PREFIX ${kdevstructuretree_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevstructuretree )

target_link_libraries(kdevstructuretree kdevquanta kdevinterfaces kdevutil ${KDE4_KDECORE_LIBS} )

install_targets(${LIB_INSTALL_DIR}/kde4 kdevstructuretree )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kdevstructuretree FILES  )
install_files( ${SERVICES_INSTALL_DIR} FILES  kdevstructuretree.desktop )
install_files( ${DATA_INSTALL_DIR}/kdevstructuretree FILES  kdevstructuretree.rc )

kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
#	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)
#
#kde_module_LTLIBRARIES = libkdevstructuretree.la
#libkdevstructuretree_la_LDFLAGS = -avoid-version -module -no-undefined \
#	-L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
#libkdevstructuretree_la_LIBADD = -lkdevquanta -lkdevelop
#libkdevstructuretree_la_SOURCES = structuretreepart.cpp structuretreewidget.cpp \
#	structuretreeglobalconfigbase.ui structuretreeglobalconfig.cpp structuretreeprojectconfigbase.ui \
#	structuretreeprojectconfig.cpp structtreeitem.cpp groupswidget.cpp groupstreeitem.cpp
#
#METASOURCES = AUTO
#
#servicedir = $(kde_servicesdir)
#service_DATA = kdevstructuretree.desktop
#
#rcdir = $(kde_datadir)/kdevstructuretree
#rc_DATA = kdevstructuretree.rc
#
#appsharedir = $(kde_datadir)/kdevstructuretree
#appshare_DATA =
#noinst_HEADERS = structtreeitem.h groupswidget.h groupstreeitem.h
#
#
#
#
#
#
