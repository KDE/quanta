
include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDevPlatform_INCLUDE_DIR}
   ${KDevPlatform_INCLUDE_DIR}/interfaces
   ${KDevPlatform_INCLUDE_DIR}/shell
   )


########### next target ###############

set(kdevtemplatestree_PART_SRCS 
   templatestreepart.cpp 
   templatestreeglobalconfig.cpp 
   templatestreeprojectconfig.cpp 
   templatestreeview.cpp 
   newtemplatedirdlg.cpp )


kde4_add_ui_files(kdevtemplatestree_PART_SRCS
   templatestreeglobalconfigbase.ui 
   templatestreeprojectconfigbase.ui 
   templatedirform.ui 
   quantapropertiespage.ui )

kde4_add_plugin(kdevtemplatestree WITH_PREFIX ${kdevtemplatestree_PART_SRCS})



target_link_libraries(kdevtemplatestree  kdevquanta kdevplatforminterfaces kdevplatformshell ${KDE4_KDE3SUPPORT_LIBS} )

install(TARGETS kdevtemplatestree  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevtemplatestree.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevtemplatestree.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevtemplatestree)


