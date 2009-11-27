
set(kdevxmlvalidator_PART_SRCS
  xmlvalidator.cpp
  xmlvalidatorplugin.cpp
  xmlschemadialog.cpp
)

kde4_add_plugin(kdevxmlvalidator ${kdevxmlvalidator_PART_SRCS})

target_link_libraries(kdevxmlvalidator
  ${KDE4_KDEUI_LIBS}
  ${KDEVPLATFORM_INTERFACES_LIBRARIES}
  xml2
)

install(TARGETS kdevxmlvalidator DESTINATION ${PLUGIN_INSTALL_DIR})

########### install files ###############

install(FILES kdevxmlvalidator.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES kdevxmlvalidator.rc DESTINATION ${DATA_INSTALL_DIR}/kdevxmlsupport)
