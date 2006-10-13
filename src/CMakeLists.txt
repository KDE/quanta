
include_directories(
  ${CMAKE_SOURCE_DIR}/quanta/lib
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDE_DIR}/kdevelop
  ${KDE4_INCLUDE_DIR}/kdevelop/interfaces
  ${KDE4_INCLUDE_DIR}/kdevelop/shell
  ${KDE4_INCLUDE_DIR}/kdevelop/shell/profileengine
  
)


########### next target ###############

set(quanta_SRCS main.cpp kdevquantaextension.cpp )

kde4_automoc(quanta ${quanta_SRCS})

kde4_add_executable(quanta ${quanta_SRCS})

target_link_libraries(quanta  ${KDE4_KDECORE_LIBS} kdevquanta kdevshell )

install(TARGETS quanta  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES  quanta.desktop  DESTINATION ${XDG_APPS_DIR})
install( FILES  profile.config  DESTINATION ${DATA_INSTALL_DIR}/kdevelop/profiles/IDE/quanta)
install( FILES  x-quanta.desktop  DESTINATION ${MIME_INSTALL_DIR}/application)
install( FILES  quantarc quantaui.rc  DESTINATION ${DATA_INSTALL_DIR}/quanta)
install( FILES  kdevelopquanta.desktop  DESTINATION ${SERVICETYPES_INSTALL_DIR})




