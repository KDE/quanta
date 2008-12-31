
find_package(Strigi REQUIRED)

include_directories(
  ${CMAKE_SOURCE_DIR}/quanta/lib
  ${CMAKE_BINARY_DIR}
  ${KDEVPLATFORM_INCLUDE_DIR}
  ${KDEVPLATFORM_INCLUDE_DIR}/interfaces
  ${KDEVPLATFORM_INCLUDE_DIR}/shell
  ${KDEVPLATFORM_INCLUDE_DIR}/shell/profileengine
  
)


########### next target ###############

set(quanta_SRCS main.cpp kdevquantaextension.cpp )


kde4_add_executable(quanta ${quanta_SRCS})

target_link_libraries(quanta  ${KDE4_KDEUI_LIBS} ${KDE4_KTEXTEDITOR_LIBRARY} ${STRIGI_LIBRARIES} ${KDEVPLATFORM_SHELL_LIBRARY} ${KDEVPLATFORM_PROJECT_LIBRARY})

install(TARGETS quanta  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES  quanta.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
install( FILES  profile.config  DESTINATION ${DATA_INSTALL_DIR}/kdevplatform/profiles/quanta)
install( FILES  quantarc quantaui.rc  DESTINATION ${DATA_INSTALL_DIR}/quanta)
install( FILES  kdevplatformquanta.desktop  DESTINATION ${SERVICETYPES_INSTALL_DIR})




