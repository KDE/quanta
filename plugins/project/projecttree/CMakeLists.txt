
include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDEVPLATFORM_INCLUDE_DIR}
   )


########### next target ###############

set(kdevprojecttree_PART_SRCS 
   projecttreepart.cpp 
   projecttreewidget.cpp 
   projecttreeglobalconfig.cpp 
   projecttreeprojectconfig.cpp 
   projecttreeview.cpp )


kde4_add_ui_files(kdevprojecttree_PART_SRCS
  projecttreeglobalconfigbase.ui
  projecttreeprojectconfigbase.ui )

kde4_add_plugin(kdevprojecttree WITH_PREFIX ${kdevprojecttree_PART_SRCS})



target_link_libraries(kdevprojecttree  kdevquanta kdevplatform kdevutil ${KDE4_KDECORE_LIBS} )

install(TARGETS kdevprojecttree  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevprojecttree.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevprojecttree.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevprojecttree)




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
#	-I$(kde_includes)/kdevplatform/interfaces -I$(kde_includes)/kdevplatform/util $(all_includes)
#
#kde_module_LTLIBRARIES = libkdevprojecttree.la
#libkdevprojecttree_la_LDFLAGS = -avoid-version -module -no-undefined \
#	-L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
#libkdevprojecttree_la_LIBADD = -lkdevquanta -lkdevplatform
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
