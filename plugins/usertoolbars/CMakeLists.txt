
add_subdirectory( pics )

include_directories(
   ${quanta_SOURCE_DIR}/lib
   ${KDEVPLATFORM_INCLUDE_DIR}
   ${KDEVPLATFORM_INCLUDE_DIR}/interfaces
   ${KDEVPLATFORM_INCLUDE_DIR}/shell
   )


########### next target ###############

set(kdevusertoolbars_PART_SRCS 
   usertoolbarspart.cpp 
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
 )

kde4_add_plugin(kdevusertoolbars WITH_PREFIX ${kdevusertoolbars_PART_SRCS})



target_link_libraries(kdevusertoolbars kdevquanta ${KDEVPLATFORM_INTERFACES_LIBRARIES} ${KDEVPLATFORM_SHELL_LIBRARIES}
   ${KDE4_KDECORE_LIBS} ${KDE4_KDE3SUPPORT_LIBS} )

install(TARGETS kdevusertoolbars  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  global  DESTINATION ${DATA_INSTALL_DIR}/quanta)
install( FILES  kdevusertoolbars.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevusertoolbars.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevusertoolbars)
install( FILES  global  DESTINATION ${DATA_INSTALL_DIR}/kdevusertoolbars)

#preferences-plugin
set(usertoolbarpreferences_PART_SRCS 
   usertoolbarsglobalconfig.cpp
)

kde4_add_ui_files(usertoolbarpreferences_PART_SRCS usertoolbarsglobalconfigbase.ui)

kde4_add_plugin(kcm_kdev_usertoolbars ${usertoolbarpreferences_PART_SRCS})

target_link_libraries(kcm_kdev_usertoolbars kdevquanta ${KDEVPLATFORM_INTERFACES_LIBRARIES} ${KDEVPLATFORM_SHELL_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS} ${QT_QT3SUPPORT_LIBRARY})

install(TARGETS kcm_kdev_usertoolbars  DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES  kcm_kdev_usertoolbars.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
