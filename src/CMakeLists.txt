
find_package(Strigi)

include_directories(
  ${CMAKE_SOURCE_DIR}/quanta/lib
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDE_DIR}/kdevplatform
  ${KDE4_INCLUDE_DIR}/kdevplatform/interfaces
  ${KDE4_INCLUDE_DIR}/kdevplatform/shell
  ${KDE4_INCLUDE_DIR}/kdevplatform/shell/profileengine
  
)


########### next target ###############

set(quanta_SRCS main.cpp kdevquantaextension.cpp )


kde4_add_executable(quanta ${quanta_SRCS})

target_link_libraries(quanta  ${KDE4_KDEUI_LIBS} ${KDE4_KTEXTEDITOR_LIBRARY} ${STRIGI_LIBRARIES} kdevplatformshell kdevplatformproject)

install(TARGETS quanta  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES  quanta.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
install( FILES  profile.config  DESTINATION ${DATA_INSTALL_DIR}/kdevplatform/profiles/quanta)
install( FILES  quantarc quantaui.rc  DESTINATION ${DATA_INSTALL_DIR}/quanta)
install( FILES  kdevplatformquanta.desktop  DESTINATION ${SERVICETYPES_INSTALL_DIR})




