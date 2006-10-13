
include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop
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

kde4_automoc(kdevstructuretree ${kdevstructuretree_PART_SRCS})

kde4_add_ui_files(kdevstructuretree_PART_SRCS
  structuretreeglobalconfigbase.ui
  structuretreeprojectconfigbase.ui )

kde4_add_plugin(kdevstructuretree WITH_PREFIX ${kdevstructuretree_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kdevstructuretree )

target_link_libraries(kdevstructuretree kdevquanta kdevplatform kdevutil ${KDE4_KDECORE_LIBS} )

install(TARGETS kdevstructuretree  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevstructuretree.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevstructuretree.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevstructuretree)



