kde4_header()

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR} 
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop
   ${KDE4_INCLUDES}
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

target_link_libraries(kdevtemplatestree  kdevquanta kdevinterfaces kdevutil kde3support ${KDE4_KDECORE_LIBS} )

install_targets(${LIB_INSTALL_DIR}/kde4 kdevtemplatestree )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kdevtemplatestree FILES  )
install_files( ${SERVICES_INSTALL_DIR} FILES  kdevtemplatestree.desktop )
install_files( ${DATA_INSTALL_DIR}/kdevtemplatestree FILES  kdevtemplatestree.rc )

kde4_footer()



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
