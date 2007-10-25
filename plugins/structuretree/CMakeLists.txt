
include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevplatform
   ${KDE4_INCLUDE_DIR}/kdevplatform/interfaces
   ${KDE4_INCLUDE_DIR}/kdevplatform/shell
   )


########### next target ###############

set(kdevstructuretree_PART_SRCS 
   structuretreepart.cpp 
   structuretreewidget.cpp 
   structuretreeglobalconfig.cpp 
   structuretreeprojectconfig.cpp 
   structtreeitem.cpp 
   groupswidget.cpp 
   groupstreeitem.cpp )


kde4_add_ui_files(kdevstructuretree_PART_SRCS
  structuretreeglobalconfigbase.ui
  structuretreeprojectconfigbase.ui )

kde4_add_plugin(kdevstructuretree WITH_PREFIX ${kdevstructuretree_PART_SRCS})



target_link_libraries(kdevstructuretree kdevquanta kdevplatforminterfaces ${KDE4_KDE3SUPPORT_LIBS} ${KDE4_KDECORE_LIBS} )

install(TARGETS kdevstructuretree  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevstructuretree.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevstructuretree.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevstructuretree)



