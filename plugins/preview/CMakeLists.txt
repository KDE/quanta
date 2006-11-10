
include_directories(
   ${KDE4_INCLUDE_DIR}/kdevelop
   )


########### next target ###############

set(kdevhtmlpreview_PART_SRCS 
   htmlpreviewpart.cpp 
   htmlpreviewglobalconfig.cpp 
   htmlpreviewprojectconfig.cpp 
   htmlpart.cpp )

kde4_automoc(${kdevhtmlpreview_PART_SRCS})

kde4_add_ui_files(kdevhtmlpreview_PART_SRCS
  htmlpreviewglobalconfigbase.ui
  htmlpreviewprojectconfigbase.ui )

kde4_add_plugin(kdevhtmlpreview WITH_PREFIX ${kdevhtmlpreview_PART_SRCS})



target_link_libraries(kdevhtmlpreview kdevplatform kdevwidgets kdevutil ktexteditor ${KDE4_KHTML_LIBS} )

install(TARGETS kdevhtmlpreview  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevhtmlpreview.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevhtmlpreview.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevhtmlpreview)



