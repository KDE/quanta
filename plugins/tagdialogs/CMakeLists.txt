kde4_header()

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR} 
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop/interfaces
   ${KDE4_INCLUDES}  
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

kde4_add_ui3_files(kdevtagdialogs_PART_SRCS tagmail.ui tagmisc.ui )

kde4_add_plugin(kdevtagdialogs WITH_PREFIX ${kdevtagdialogs_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevtagdialogs )

target_link_libraries(kdevtagdialogs  kdevquanta kdevinterfaces kabc ${KDE4_KDECORE_LIBS} )

install_targets(${LIB_INSTALL_DIR}/kde4 kdevtagdialogs )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kdevtagdialogs FILES  )
install_files( ${SERVICES_INSTALL_DIR} FILES  kdevtagdialogs.desktop )
install_files( ${DATA_INSTALL_DIR}/kdevtagdialogs FILES  kdevtagdialogs.rc )
#install_files( ${SERVICETYPES_INSTALL_DIR} FILES  kdeveloptagdialogs.desktop )

kde4_footer()



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
