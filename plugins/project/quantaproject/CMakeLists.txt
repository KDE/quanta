kde4_header()

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR} 
   ${CMAKE_CURRENT_BINARY_DIR} 
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop
   ${KDE4_INCLUDES}  
   )


########### next target ###############

set(kdevquantaproject_PART_SRCS 
   quantaprojectpart.cpp 
   quantaprojectwidget.cpp 
   quantaprojectprojectconfig.cpp 
   browserpopupmenu.cpp )

kde4_automoc(${kdevquantaproject_PART_SRCS})

kde4_add_ui_files(kdevquantaproject_PART_SRCS
  quantaprojectprojectconfigbase.ui )

kde4_add_plugin(kdevquantaproject WITH_PREFIX ${kdevquantaproject_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevquantaproject )

target_link_libraries(kdevquantaproject  kdevquanta kdevinterfaces kdevutil ${KDE4_KDECORE_LIBS} )

install_targets(${LIB_INSTALL_DIR}/kde4 kdevquantaproject )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kdevquantaproject FILES  )
install_files( ${SERVICES_INSTALL_DIR} FILES  kdevquantaproject.desktop )
install_files( ${DATA_INSTALL_DIR}/kdevquantaproject FILES  kdevquantaproject.rc )

kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/lib -I$(kde_includes)/kdevelop/interfaces \
#	-I$(kde_includes)/kdevelop/util $(all_includes)
#
#kde_module_LTLIBRARIES = libkdevquantaproject.la
#libkdevquantaproject_la_LDFLAGS = -L$(top_builddir)/lib -avoid-version -module -no-undefined \
#	$(all_libraries) $(KDE_PLUGIN)
#libkdevquantaproject_la_LIBADD = -lkdevquanta	-lkdevelop
#libkdevquantaproject_la_SOURCES = quantaprojectpart.cpp quantaprojectwidget.cpp \
#		quantaprojectprojectconfigbase.ui quantaprojectprojectconfig.cpp browserpopupmenu.cpp
#
#METASOURCES = AUTO
#
#servicedir = $(kde_servicesdir)
#service_DATA = kdevquantaproject.desktop
#
#rcdir = $(kde_datadir)/kdevquantaproject
#rc_DATA = kdevquantaproject.rc
#
#appsharedir = $(kde_datadir)/kdevquantaproject
#appshare_DATA =
#noinst_HEADERS = browserpopupmenu.h
