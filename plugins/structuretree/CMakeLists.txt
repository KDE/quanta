
include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop
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

kde4_add_ui_files(kdevstructuretree_PART_SRCS
  structuretreeglobalconfigbase.ui
  structuretreeprojectconfigbase.ui )

kde4_add_plugin(kdevstructuretree WITH_PREFIX ${kdevstructuretree_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevstructuretree )

target_link_libraries(kdevstructuretree kdevquanta kdevinterfaces kdevutil ${KDE4_KDECORE_LIBS} )

install(TARGETS kdevstructuretree  DESTINATION ${LIB_INSTALL_DIR}/kde4)


########### install files ###############

install( FILES   DESTINATION ${DATA_INSTALL_DIR}/kdevstructuretree)
install( FILES  kdevstructuretree.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevstructuretree.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevstructuretree)




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
