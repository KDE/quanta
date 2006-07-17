
include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop
   )


########### next target ###############

set(kdevtagdialogs_PART_SRCS 
   tagdialogspart.cpp 
   colorcombo.cpp 
   listdlg.cpp 
   pictureview.cpp 
   tagdialog.cpp 
   tagimgdlg.cpp 
   tagimgdlgdata.cpp 
   tagmaildlg.cpp 
   tagmiscdlg.cpp 
   tagquicklistdlg.cpp 
   tagquicklistdlgdata.cpp 
   tagwidget.cpp 
   tagxml.cpp 
   attributes.cpp )

kde4_automoc(${kdevtagdialogs_PART_SRCS})

kde4_add_ui_files(kdevtagdialogs_PART_SRCS
  tagmail.ui
  tagmisc.ui )

kde4_add_plugin(kdevtagdialogs WITH_PREFIX ${kdevtagdialogs_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevtagdialogs )

target_link_libraries(kdevtagdialogs  kdevquanta kdevinterfaces ${KDE4_KDECORE_LIBS} )

install(TARGETS kdevtagdialogs  DESTINATION ${PLUGIN_INSTALL_DIR}/kde4)


########### install files ###############

install( FILES  kdevtagdialogs.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevtagdialogs.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevtagdialogs)




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/lib -I$(top_builddir)/lib \
#	-I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util $(all_includes)
#
#kde_module_LTLIBRARIES = libkdevtagdialogs.la
#libkdevtagdialogs_la_LDFLAGS = -avoid-version -module -no-undefined \
#	-L$(top_builddir)/lib $(all_libraries) $(KDE_PLUGIN)
#libkdevtagdialogs_la_LIBADD = -lkdevquanta -lkdevelop
#libkdevtagdialogs_la_SOURCES = tagdialogspart.cpp colorcombo.cpp listdlg.cpp \
#	pictureview.cpp tagdialog.cpp tagimgdlg.cpp tagimgdlgdata.cpp tagmaildlg.cpp tagmail.ui \
#	tagmiscdlg.cpp tagmisc.ui tagquicklistdlg.cpp tagquicklistdlgdata.cpp tagwidget.cpp \
#	tagxml.cpp attributes.cpp
#
#METASOURCES = AUTO
#
#servicedir = $(kde_servicesdir)
#service_DATA = kdevtagdialogs.desktop
#
#servicetypedir = $(kde_servicetypesdir)
#servicetype_DATA = kdeveloptagdialogs.desktop
#
#rcdir = $(kde_datadir)/kdevtagdialogs
#rc_DATA = kdevtagdialogs.rc
#
#appsharedir = $(kde_datadir)/kdevtagdialogs
#appshare_DATA =
#noinst_HEADERS = tagdialogspart.h colorcombo.h listdlg.h pictureview.h \
#	tagdialog.h tagimgdlg.h tagmaildlg.h tagmiscdlg.h tagquicklistdlg.h tagwidget.h \
#	tagxml.h attributes.h
#
