
include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop
   )


########### next target ###############

set(kdevtemplatestree_PART_SRCS 
   templatestreepart.cpp 
   templatestreeglobalconfig.cpp 
   templatestreeprojectconfig.cpp 
   templatestreeview.cpp 
   newtemplatedirdlg.cpp )

kde4_automoc(${kdevtemplatestree_PART_SRCS})

kde4_add_ui_files(kdevtemplatestree_PART_SRCS
   templatestreeglobalconfigbase.ui 
   templatestreeprojectconfigbase.ui 
   templatedirform.ui 
   quantapropertiespage.ui )

kde4_add_plugin(kdevtemplatestree WITH_PREFIX ${kdevtemplatestree_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevtemplatestree )

target_link_libraries(kdevtemplatestree  kdevquanta kdevinterfaces kdevutil ${KDE4_KDE3SUPPORT_LIBS} )

install(TARGETS kdevtemplatestree  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevtemplatestree.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevtemplatestree.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevtemplatestree)




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
#	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)
#
#kde_module_LTLIBRARIES = libkdevtemplatestree.la
#libkdevtemplatestree_la_LDFLAGS = -avoid-version -module -no-undefined \
#	-L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
#libkdevtemplatestree_la_LIBADD = -lkdevquanta -lkdevelop
#libkdevtemplatestree_la_SOURCES = templatestreepart.cpp \
#	templatestreeglobalconfigbase.ui templatestreeglobalconfig.cpp templatestreeprojectconfigbase.ui \
#	templatestreeprojectconfig.cpp templatedirform.ui templatestreeview.cpp newtemplatedirdlg.cpp \
#	quantapropertiespage.ui
#
#METASOURCES = AUTO
#
#servicedir = $(kde_servicesdir)
#service_DATA = kdevtemplatestree.desktop
#
#rcdir = $(kde_datadir)/kdevtemplatestree
#rc_DATA = kdevtemplatestree.rc
#
#appsharedir = $(kde_datadir)/kdevtemplatestree
#appshare_DATA =
#noinst_HEADERS = templatestreeview.h newtemplatedirdlg.h
