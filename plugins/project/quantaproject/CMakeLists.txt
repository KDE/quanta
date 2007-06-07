
include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevplatform
   ${KDE4_INCLUDE_DIR}/kdevplatform/util
   )


########### next target ###############

set(kdevquantaproject_PART_SRCS
   quantafilemanager.cpp
   quantaprojectpart.cpp 
   quantaprojectmanager.cpp 
   quantaprojectprojectconfig.cpp 
   browserpopupmenu.cpp )

kde4_automoc(${kdevquantaproject_PART_SRCS})

kde4_add_ui_files(kdevquantaproject_PART_SRCS
  quantaprojectprojectconfigbase.ui )

kde4_add_plugin(kdevquantaproject WITH_PREFIX ${kdevquantaproject_PART_SRCS})



target_link_libraries(kdevquantaproject  kdevquanta kdevplatform kdevutil ${KDE4_KDECORE_LIBS} )

install(TARGETS kdevquantaproject  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevquantaproject.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevquantaproject.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevquantaproject)




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/lib -I$(kde_includes)/kdevplatform/interfaces \
#	-I$(kde_includes)/kdevplatform/util $(all_includes)
#
#kde_module_LTLIBRARIES = libkdevquantaproject.la
#libkdevquantaproject_la_LDFLAGS = -L$(top_builddir)/lib -avoid-version -module -no-undefined \
#	$(all_libraries) $(KDE_PLUGIN)
#libkdevquantaproject_la_LIBADD = -lkdevquanta	-lkdevplatform
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
