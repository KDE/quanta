
include_directories(
   ${quanta_BINARY_DIR}/..//lib
   ${KDEVPLATFORM_INCLUDE_DIR}
   ${KDEVPLATFORM_INCLUDE_DIR}/interfaces
   ${KDEVPLATFORM_INCLUDE_DIR}/shell
   ${KDEVPLATFORM_INCLUDE_DIR}/editor
   )


########### next target ###############

set(kdevquanta_LIB_SRCS 
   settings.cpp 
   resizablewidget.cpp 
   useraction.cpp 
   myprocess.cpp 
#   node.cpp 
#   tag.cpp 
   qtag.cpp 
#   tagattr.cpp 
   editorsource.cpp
   fileinfodlg.cpp
   filecombo.cpp 
   tagdialogsif.cpp 
   basetreeview.cpp 
   helper.cpp 
   quantanetaccess.cpp 
   tagpair.cpp 
#   groupstorage.cpp 
   extfileinfo.cpp 
   quantaprojectif.cpp 
   searchlinewidget.cpp
   dommodel.cpp
   )


kde4_add_ui_files(kdevquanta_LIB_SRCS fileinfodlg.ui )

kde4_add_kcfg_files(kdevquanta_LIB_SRCS settingsbase.kcfgc )

kde4_add_library(kdevquanta SHARED ${kdevquanta_LIB_SRCS})

target_link_libraries(kdevquanta ${KDEVPLATFORM_INTERFACES_LIBRARIES} ${KDEVPLATFORM_SHELL_LIBRARIES} ${KDEVPLATFORM_LANGUAGE_LIBRARIES} ${KDE4_KTEXTEDITOR_LIBS} ${KDE4_KFILE_LIBS} ${KDE4_KDE3SUPPORT_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS})

set_target_properties(kdevquanta PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install(TARGETS kdevquanta ${INSTALL_TARGETS_DEFAULT_ARGS})


########### install files ###############

install( FILES  quanta.kcfg  DESTINATION ${KCFG_INSTALL_DIR})

