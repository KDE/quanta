include_directories(
  ${quanta_SOURCE_DIR}/lib
  ${quanta_BINARY_DIR}/../
  ${KDEVPLATFORM_INCLUDE_DIR}
  ${KDEVPLATFORM_INCLUDE_DIR}/interfaces
  ${KDEVPLATFORM_INCLUDE_DIR}/shell
  ${KDEVPLATFORM_INCLUDE_DIR}/shell/profileengine
  
)

########### main binaries ###############

find_package(SharedMimeInfo REQUIRED)

set(quanta_SRCS main.cpp
    kdevquantaextension.cpp)
kde4_add_executable(quanta ${quanta_SRCS} )

target_link_libraries(quanta
    ${KDE4_KDECORE_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_SHELL_LIBRARIES}
    ${KDE4_KIO_LIBS}
    ${KDE4_KUTILS_LIBS}
    ${KDE4_KPARTS_LIBS}
    ${KDE4_KNOTIFYCONFIG_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${KDE4_THREADWEAVER_LIBRARIES}
)
install(TARGETS quanta ${INSTALL_TARGETS_DEFAULT_ARGS} )

kde4_add_app_icon(quanta_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../data/pics/hi*-app-quanta.png")


if (Q_WS_MAC)
    #sets MacOSX bundle properties for all targets in this cmake file
    #make those assignments target-dependent when cmake 2.6 is useful on MacOSX
    set(MACOSX_BUNDLE_BUNDLE_NAME Quanta)
    set(MACOSX_BUNDLE_BUNDLE_VERSION "4.0")
    set(MACOSX_BUNDLE_COPYRIGHT "Quanta Team")
endif (Q_WS_MAC)


########### install files ###############

install( PROGRAMS  quanta.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
install( FILES  profile.config  DESTINATION ${DATA_INSTALL_DIR}/kdevplatform/profiles/quanta)
install( FILES  quantarc quantaui.rc  DESTINATION ${DATA_INSTALL_DIR}/quanta)
install( FILES  kdevplatformquanta.desktop  DESTINATION ${SERVICETYPES_INSTALL_DIR})




