
set(kdevxmlformatter_PART_SRCS
  xmlformatter.cpp
  xmlformatterplugin.cpp
  xmlformatterpreferences.cpp
)

kde4_add_ui_files(kdevxmlformatter_PART_SRCS xmlformatterwidget.ui)

kde4_add_plugin(kdevxmlformatter ${kdevxmlformatter_PART_SRCS})

target_link_libraries(kdevxmlformatter
  ${KDE4_KDEUI_LIBS}
  ${KDEVPLATFORM_INTERFACES_LIBRARIES}
)

install(TARGETS kdevxmlformatter DESTINATION ${PLUGIN_INSTALL_DIR})

########### install files ###############

install(FILES kdevxmlformatter.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES kdevxmlformatter.rc DESTINATION ${DATA_INSTALL_DIR}/kdevxmlsupport)
