kde4_header()

add_subdirectory( pics )

include_directories(
  ${CMAKE_SOURCE_DIR}/quanta/lib
  ${CMAKE_BINARY_DIR}/quanta/plugins/usertoolbars
  ${KDE4_INCLUDE_DIR}/kdevelop/interfaces
  ${KDE4_INCLUDE_DIR}/kdevelop/util
  ${KDE4_INCLUDES}
)


########### next target ###############

set(kdevusertoolbars_PART_SRCS 
   usertoolbarspart.cpp 
   usertoolbarsglobalconfig.cpp 
   toolbarxmlgui.cpp 
   toolbartabwidget.cpp 
   toolbarguibuilder.cpp 
   usertoolbar.cpp 
   askforsavedlg.cpp 
   maildialog.cpp 
   newtoolbarstuff.cpp 
   actionconfigdialog.cpp )

kde4_automoc(${kdevusertoolbars_PART_SRCS})

kde4_add_ui_files(kdevusertoolbars_PART_SRCS
  actionconfigdialogbase.ui
)

kde4_add_ui3_files(kdevusertoolbars_PART_SRCS usertoolbarsglobalconfigbase.ui maildialogbase.ui)

kde4_add_plugin(kdevusertoolbars WITH_PREFIX ${kdevusertoolbars_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevusertoolbars )

target_link_libraries(kdevusertoolbars kdevquanta kdevinterfaces kdevutil
kabc knewstuff  ${KDE4_KDECORE_LIBS} )

install_targets(${LIB_INSTALL_DIR}/kde4 kdevusertoolbars )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kdevusertoolbars FILES  )
install_files( ${DATA_INSTALL_DIR}/quanta FILES  global )
install_files( ${SERVICES_INSTALL_DIR} FILES  kdevusertoolbars.desktop )
install_files( ${DATA_INSTALL_DIR}/kdevusertoolbars FILES  kdevusertoolbars.rc )
install_files( ${DATA_INSTALL_DIR}/kdevusertoolbars FILES  global )

kde4_footer()



#original Makefile.am contents follow:

#AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)
#
#kde_module_LTLIBRARIES = libkdevusertoolbars.la
#libkdevusertoolbars_la_LDFLAGS = -module -avoid-version -no-undefined -L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
#libkdevusertoolbars_la_LIBADD = -lkdevquanta -lkdevelop -lkdevextras -lknewstuff 
#libkdevusertoolbars_la_SOURCES = usertoolbarspart.cpp \
#	usertoolbarsglobalconfigbase.ui usertoolbarsglobalconfig.cpp toolbarxmlgui.cpp toolbartabwidget.cpp \
#	toolbarguibuilder.cpp usertoolbar.cpp askforsavedlg.cpp maildialogbase.ui maildialog.cpp \
#	newtoolbarstuff.cpp actionconfigdialogbase.ui actionconfigdialog.cpp
#
#METASOURCES = AUTO
#
#servicedir = $(kde_servicesdir)
#service_DATA = kdevusertoolbars.desktop
#
#rcdir = $(kde_datadir)/kdevusertoolbars
#rc_DATA = kdevusertoolbars.rc
#
#globaldir = $(kde_datadir)/kdevusertoolbars
#global_DATA = global
#
#quantadir = $(kde_datadir)/quanta
#quanta_DATA = global
#
#appsharedir = $(kde_datadir)/kdevusertoolbars
#appshare_DATA =
#noinst_HEADERS = toolbarguibuilder.h askforsavedlg.h newtoolbarstuff.h
#
#SUBDIRS = pics
