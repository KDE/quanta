
add_subdirectory( pics )

include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${KDEVPLATFORM_INCLUDE_DIR}
   ${KDEVPLATFORM_INCLUDE_DIR}/interfaces
   ${KDEVPLATFORM_INCLUDE_DIR}/shell
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


kde4_add_ui_files(
   kdevusertoolbars_PART_SRCS
   actionconfigdialogbase.ui
   maildialogbase.ui
   usertoolbarsglobalconfigbase.ui
 )

kde4_add_plugin(kdevusertoolbars WITH_PREFIX ${kdevusertoolbars_PART_SRCS})



target_link_libraries(kdevusertoolbars kdevquanta kdevplatforminterfaces
   ${KDE4_KDECORE_LIBS} ${KDE4_KDE3SUPPORT_LIBS} )

install(TARGETS kdevusertoolbars  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  global  DESTINATION ${DATA_INSTALL_DIR}/quanta)
install( FILES  kdevusertoolbars.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevusertoolbars.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevusertoolbars)
install( FILES  global  DESTINATION ${DATA_INSTALL_DIR}/kdevusertoolbars)

