
include_directories(
   ${CMAKE_SOURCE_DIR}/quanta/lib
   ${CMAKE_BINARY_DIR}/quanta/lib
   ${KDE4_INCLUDE_DIR}/kdevelop
   ${KDE4_INCLUDE_DIR}/kdevelop/interfaces
   ${KDE4_INCLUDE_DIR}/kdevelop/shell
   )


########### next target ###############

set(kdevquantafilestree_PART_SRCS 
   quantafilestreepart.cpp 
   quantafilestreeglobalconfig.cpp 
   quantafilestreeprojectconfig.cpp 
   filestreeview.cpp )

kde4_automoc(${kdevquantafilestree_PART_SRCS})

kde4_add_ui_files(kdevquantafilestree_PART_SRCS
  quantafilestreeglobalconfigbase.ui
  quantafilestreeprojectconfigbase.ui )

kde4_add_plugin(kdevquantafilestree WITH_PREFIX ${kdevquantafilestree_PART_SRCS})



target_link_libraries(kdevquantafilestree  kdevquanta kdevplatforminterfaces kde3support ${KDE4_KDECORE_LIBS} )

install(TARGETS kdevquantafilestree  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevquantafilestree.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevquantafilestree.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevquantafilestree)


