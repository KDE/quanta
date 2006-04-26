kde4_header()

add_subdirectory( pics )

include_directories(
  ${CMAKE_SOURCE_DIR}/quanta/lib
  ${CMAKE_SOURCE_DIR}/quanta/plugins/createproject
  ${CMAKE_BINARY_DIR}/quanta/lib
  ${CMAKE_BINARY_DIR}/quanta/plugins/createproject
  ${KDE4_INCLUDE_DIR}/kdevelop/interfaces
  ${KDE4_INCLUDES}
)


########### next target ###############

set(kdevcreatequantaproject_PART_SRCS 
   createquantaprojectpart.cpp 
   simplefiletree.cpp 
   simpletreeviewitem.cpp 
   firstwizardpage.cpp 
   localimportwizardpage.cpp 
   secondwizardpage.cpp 
   webimportwizardpage.cpp )

kde4_automoc(${kdevcreatequantaproject_PART_SRCS})

kde4_add_ui3_files(kdevcreatequantaproject_PART_SRCS 
   firstwizardpagebase.ui 
   localimportwizardpagebase.ui 
   secondwizardpagebase.ui 
   webimportwizardpagebase.ui
)

kde4_add_plugin(kdevcreatequantaproject WITH_PREFIX ${kdevcreatequantaproject_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevcreatequantaproject )

target_link_libraries(kdevcreatequantaproject kdevquanta kdevinterfaces ${KDE4_KDECORE_LIBS})

install_targets(${LIB_INSTALL_DIR}/kde4 kdevcreatequantaproject )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kdevcreatequantaproject FILES  )
install_files( ${SERVICES_INSTALL_DIR} FILES  kdevcreatequantaproject.desktop )
install_files( ${DATA_INSTALL_DIR}/kdevcreatequantaproject FILES  kdevcreatequantaproject.rc quanta-project.template )

kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
#	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)
#
#kde_module_LTLIBRARIES = libkdevcreatequantaproject.la
#libkdevcreatequantaproject_la_LDFLAGS = -L$(top_builddir)/lib -avoid-version -module -no-undefined \
#	$(all_libraries) $(KDE_PLUGIN)
#libkdevcreatequantaproject_la_LIBADD = -lkdevelop -lkdevquanta
#libkdevcreatequantaproject_la_SOURCES = createquantaprojectpart.cpp \
#	simplefiletree.cpp simpletreeviewitem.cpp firstwizardpagebase.ui firstwizardpage.cpp \
#	localimportwizardpagebase.ui localimportwizardpage.cpp secondwizardpagebase.ui secondwizardpage.cpp \
#	webimportwizardpagebase.ui webimportwizardpage.cpp
#
#METASOURCES = AUTO
#
#servicedir = $(kde_servicesdir)
#service_DATA = kdevcreatequantaproject.desktop
#
#rcdir = $(kde_datadir)/kdevcreatequantaproject
#rc_DATA = kdevcreatequantaproject.rc quanta-project.template
#
#appsharedir = $(kde_datadir)/kdevcreatequantaproject
#appshare_DATA =
#noinst_HEADERS = simplefiletree.h simpletreeviewitem.h firstwizardpage.h \
#	localimportwizardpage.h secondwizardpage.h webimportwizardpage.h
#
#SUBDIRS = pics
