include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevplatform
   ${KDE4_INCLUDE_DIR}/kdevplatform/interfaces
   ${KDE4_INCLUDE_DIR}/kdevplatform/project
   )

########### next target ###############

set(quantaprojectfilemanager_PART_SRCS 
   quantaprojectfilemanager.cpp 
   )

kde4_automoc(${quantaprojectfilemanager_PART_SRCS})

#kde4_add_ui_files(quantaprojectfilemanager_PART_SRCS
#  projecttreeglobalconfigbase.ui
#  projecttreeprojectconfigbase.ui )

kde4_add_plugin(quantaprojectfilemanager ${quantaprojectfilemanager_PART_SRCS})

target_link_libraries(quantaprojectfilemanager kdevquanta kdevplatforminterfaces kdevplatformproject ${KDE4_KDECORE_LIBS} )

install(TARGETS quantaprojectfilemanager  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  quantaprojectfilemanager.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  quantaprojectfilemanager.rc  DESTINATION ${DATA_INSTALL_DIR}/quantaprojectfilemanager)

