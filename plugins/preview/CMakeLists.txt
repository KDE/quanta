kde4_header()

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR} 
   ${CMAKE_CURRENT_BINARY_DIR}
   ${KDE4_INCLUDE_DIR}/kdevelop/interfaces
   ${KDE4_INCLUDE_DIR}/kdevelop/widgets
   ${KDE4_INCLUDE_DIR}/kdevelop/util
   ${KDE4_INCLUDES}  
   )


########### next target ###############

set(kdevhtmlpreview_PART_SRCS 
   htmlpreviewpart.cpp 
   htmlpreviewglobalconfig.cpp 
   htmlpreviewprojectconfig.cpp 
   htmlpart.cpp )

kde4_automoc(${kdevhtmlpreview_PART_SRCS})

kde4_add_ui_files(kdevhtmlpreview_PART_SRCS
  htmlpreviewglobalconfigbase.ui
  htmlpreviewprojectconfigbase.ui )

kde4_add_plugin(kdevhtmlpreview WITH_PREFIX ${kdevhtmlpreview_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevhtmlpreview )

target_link_libraries(kdevhtmlpreview kdevinterfaces kdevwidgets kdevutil ktexteditor ${KDE4_KHTML_LIBS} )

install_targets(${LIB_INSTALL_DIR}/kde4 kdevhtmlpreview )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kdevhtmlpreview FILES  )
install_files( ${SERVICES_INSTALL_DIR} FILES  kdevhtmlpreview.desktop )
install_files( ${DATA_INSTALL_DIR}/kdevhtmlpreview FILES  kdevhtmlpreview.rc )

kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(kde_includes)/kdevelop/widgets \
#	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)
#
#kde_module_LTLIBRARIES = libkdevhtmlpreview.la
#libkdevhtmlpreview_la_LDFLAGS = -avoid-version -module -no-undefined \
#	$(all_libraries) $(KDE_PLUGIN)
#libkdevhtmlpreview_la_LIBADD = -lkdevwidgets -lkdevelop $(LIB_KHTML)
#libkdevhtmlpreview_la_SOURCES = htmlpreviewpart.cpp \
#	htmlpreviewglobalconfigbase.ui htmlpreviewglobalconfig.cpp htmlpreviewprojectconfigbase.ui \
#	htmlpreviewprojectconfig.cpp htmlpart.cpp
#
#METASOURCES = AUTO
#
#servicedir = $(kde_servicesdir)
#service_DATA = kdevhtmlpreview.desktop
#
#rcdir = $(kde_datadir)/kdevhtmlpreview
#rc_DATA = kdevhtmlpreview.rc
#
#appsharedir = $(kde_datadir)/kdevhtmlpreview
#appshare_DATA =
#noinst_HEADERS = htmlpart.h
