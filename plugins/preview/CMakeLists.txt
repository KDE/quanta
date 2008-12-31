
include_directories(
   ${KDEVPLATFORM_INCLUDE_DIR}
   ${KDEVPLATFORM_INCLUDE_DIR}/interfaces
   ${KDEVPLATFORM_INCLUDE_DIR}/shell

   )


########### next target ###############

set(kdevhtmlpreview_PART_SRCS 
   htmlpreviewpart.cpp 
   htmlpreviewglobalconfig.cpp 
   htmlpreviewprojectconfig.cpp 
   htmlpart.cpp )


kde4_add_ui_files(kdevhtmlpreview_PART_SRCS
  htmlpreviewglobalconfigbase.ui
  htmlpreviewprojectconfigbase.ui )

kde4_add_plugin(kdevhtmlpreview WITH_PREFIX ${kdevhtmlpreview_PART_SRCS})



target_link_libraries(kdevhtmlpreview ${KDEVPLATFORM_INTERFACES_LIBRARY} ${KDEVPLATFORM_SHELL_LIBRARY} ${KDE4_KTEXTEDITOR_LIBS} ${KDE4_KHTML_LIBS} )

install(TARGETS kdevhtmlpreview  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevhtmlpreview.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevhtmlpreview.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevhtmlpreview)



