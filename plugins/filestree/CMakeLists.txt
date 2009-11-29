
include_directories(
   ${quanta_SOURCE_DIR}/lib
   ${quanta_BINARY_DIR}/lib
   ${KDEVPLATFORM_INCLUDE_DIR}
   ${KDEVPLATFORM_INCLUDE_DIR}/interfaces
   ${KDEVPLATFORM_INCLUDE_DIR}/shell
   )


########### next target ###############

set(kdevquantafilestree_PART_SRCS 
   quantafilestreepart.cpp 
   quantafilestreeglobalconfig.cpp 
   quantafilestreeprojectconfig.cpp 
   filestreeview.cpp )


kde4_add_ui_files(kdevquantafilestree_PART_SRCS
  quantafilestreeglobalconfigbase.ui
  quantafilestreeprojectconfigbase.ui )

kde4_add_plugin(kdevquantafilestree WITH_PREFIX ${kdevquantafilestree_PART_SRCS})



target_link_libraries(kdevquantafilestree  kdevquanta kdevplatforminterfaces kdevplatformshell ${KDE4_KDE3SUPPORT_LIBS} ${KDE4_KDECORE_LIBS} )

install(TARGETS kdevquantafilestree  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES  kdevquantafilestree.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES  kdevquantafilestree.rc  DESTINATION ${DATA_INSTALL_DIR}/kdevquantafilestree)


