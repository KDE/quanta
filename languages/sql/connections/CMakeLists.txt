include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(kcm_kdev_sqlconnections_SRCS
   connections.cpp
   connectionsmodel.cpp
)
kde4_add_ui_files( kcm_kdev_sqlconnections_SRCS connections.ui )
kde4_add_plugin(kcm_kdev_sqlconnections ${kcm_kdev_sqlconnections_SRCS})

target_link_libraries(kcm_kdev_sqlconnections
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${QT_QTSQL_LIBRARY}
)
install(TARGETS kcm_kdev_sqlconnections  DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES  kcm_kdev_sqlconnections.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
