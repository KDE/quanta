kde4_header()

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_BINARY_DIR}/lib
   ${KDE4_INCLUDE_DIR}/kdevelop/interfaces
   ${KDE4_INCLUDES} 
   )


########### next target ###############

set(kdevquanta_LIB_SRCS 
   settings.cpp 
   resizablewidget.cpp 
   useraction.cpp 
   quantacoreif.cpp 
   myprocess.cpp 
   node.cpp 
   tag.cpp 
   qtag.cpp 
   tagattr.cpp 
   editorsource.cpp
   fileinfodlg.cpp
   filecombo.cpp 
   tagdialogsif.cpp 
   basetreeview.cpp 
   helper.cpp 
   quantanetaccess.cpp 
   tagpair.cpp 
   groupstorage.cpp 
   extfileinfo.cpp 
   quantaprojectif.cpp 
   searchlinewidget.cpp 
   )

kde4_automoc(${kdevquanta_LIB_SRCS})

kde4_add_ui_files(kdevquanta_LIB_SRCS fileinfodlg.ui )

kde4_add_kcfg_files(kdevquanta_LIB_SRCS settingsbase.kcfgc )

kde4_add_library(kdevquanta SHARED ${kdevquanta_LIB_SRCS})

target_link_libraries(kdevquanta kdevinterfaces ktexteditor ${KDE4_KIO_LIBS})

set_target_properties(kdevquanta PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} kdevquanta )


########### install files ###############

install_files( ${KCFG_INSTALL_DIR} FILES  quanta.kcfg )

kde4_footer()



#original Makefile.am contents follow:

#lib_LTLIBRARIES = libkdevquanta.la
#libkdevquanta_la_SOURCES = settings.cpp settingsbase.kcfgc resizablewidget.cpp \
#	useraction.cpp quantacoreif.cpp myprocess.cpp node.cpp tag.cpp qtag.cpp tagattr.cpp \
#	editorsource.cpp filecombo.cpp tagdialogsif.cpp basetreeview.cpp fileinfodlg.ui \
#	helper.cpp quantanetaccess.cpp tagpair.cpp groupstorage.cpp extfileinfo.cpp \
#	quantaprojectif.cpp searchlinewidget.cpp
#
#AM_CPPFLAGS = $(all_includes)
#METASOURCES = AUTO
#
#noinst_HEADERS = resizablewidget.h useraction.h quantacoreif.h node.h tag.h \
#	areastruct.h qtag.h tagattr.h dtdstruct.h parseresult.h editorsource.h filecombo.h \
#	tagdialogsif.h basetreeview.h helper.h quantanetaccess.h groupstorage.h groupelement.h \
#	tagpair.h extfileinfo.h quantaprojectif.h filterableitemif.h searchlinewidget.h \
#	settings.h
#libkdevquanta_la_LDFLAGS = -avoid-version
#INCLUDES = -I$(top_srcdir)/lib -I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/util
#
#kde_kcfg_DATA = quanta.kcfg
