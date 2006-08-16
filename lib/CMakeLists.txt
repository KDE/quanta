
include_directories(
   ${CMAKE_BINARY_DIR}/lib
   ${KDE4_INCLUDE_DIR}/kdevelop
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
   hacks.cpp
   )

kde4_automoc(${kdevquanta_LIB_SRCS})

kde4_add_ui_files(kdevquanta_LIB_SRCS fileinfodlg.ui )

kde4_add_kcfg_files(kdevquanta_LIB_SRCS settingsbase.kcfgc )

kde4_add_library(kdevquanta SHARED ${kdevquanta_LIB_SRCS})

target_link_libraries(kdevquanta kdevplatform ktexteditor ${KDE4_KIO_LIBS})

set_target_properties(kdevquanta PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install(TARGETS kdevquanta  DESTINATION ${LIB_INSTALL_DIR})


########### install files ###############

install( FILES  quanta.kcfg  DESTINATION ${KCFG_INSTALL_DIR})

