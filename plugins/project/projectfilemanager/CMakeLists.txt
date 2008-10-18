include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDevPlatform_INCLUDE_DIR}
   ${KDevPlatform_INCLUDE_DIR}/interfaces
   ${KDevPlatform_INCLUDE_DIR}/project
   )

########### next target ###############

set(quantaprojectfilemanager_PART_SRCS 
   quantaprojectfilemanager.cpp 
   )


#kde4_add_ui_files(quantaprojectfilemanager_PART_SRCS
#  projecttreeglobalconfigbase.ui
#  projecttreeprojectconfigbase.ui )

kde4_add_plugin(quantaprojectfilemanager ${quantaprojectfilemanager_PART_SRCS})

target_link_libraries(quantaprojectfilemanager kdevquanta kdevplatforminterfaces kdevplatformproject ${KDE4_KIO_LIBS} ${QT_QTXML_LIBRARY})

install(TARGETS quantaprojectfilemanager  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  quantaprojectfilemanager.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  quantaprojectfilemanager.rc  DESTINATION ${DATA_INSTALL_DIR}/quantaprojectfilemanager)

