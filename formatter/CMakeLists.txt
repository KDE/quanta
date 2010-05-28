
set(kdevxmlformatter_PART_SRCS
  xmlformatter.cpp
  xmlformatterplugin.cpp
  xmlformatterpreferences.cpp
)

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${QT4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    kdevsgmlduchain
)

kde4_add_ui_files(kdevxmlformatter_PART_SRCS xmlformatterwidget.ui)

kde4_add_plugin(kdevxmlformatter ${kdevxmlformatter_PART_SRCS})

target_link_libraries(kdevxmlformatter
  ${KDE4_KDEUI_LIBS}
  ${KDEVPLATFORM_INTERFACES_LIBRARIES}
  kdevsgmlduchain
)

install(TARGETS kdevxmlformatter DESTINATION ${PLUGIN_INSTALL_DIR})

########### install files ###############

install(FILES kdevxmlformatter.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES kdevxmlformatter.rc DESTINATION ${DATA_INSTALL_DIR}/kdevxmlsupport)
