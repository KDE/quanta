
set(kdevxmlvalidator_SRCS
  xmlvalidator.cpp
  xmlvalidatorplugin.cpp
  xmlschemadialog.cpp
  xmlvalidatorjob.cpp
)

include_directories(${xml_SOURCE_DIR}/catalog)

kde4_add_plugin(kdevxmlvalidator ${kdevxmlvalidator_SRCS})

target_link_libraries(kdevxmlvalidator
  ${KDE4_KDEUI_LIBS}
  ${KDEVPLATFORM_INTERFACES_LIBRARIES}
  kdevxmlcatalog
  xml2
  ${KDE4_THREADWEAVER_LIBRARIES}
)

install(TARGETS kdevxmlvalidator DESTINATION ${PLUGIN_INSTALL_DIR})

########### install files ###############

install(FILES kdevxmlvalidator.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES kdevxmlvalidator.rc DESTINATION ${DATA_INSTALL_DIR}/kdevxmlsupport)
