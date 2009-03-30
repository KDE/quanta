project(xdebug)

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=9046 )


enable_testing()
find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

########### next target ###############

set(kdevxdebugcore_PART_SRCS
    connection.cpp
    debugsession.cpp
    server.cpp
    stackmodel.cpp
)

set(kdevxdebug_PART_SRCS
    xdebugplugin.cpp
)

kde4_add_plugin(kdevxdebug ${kdevxdebug_PART_SRCS} ${kdevxdebugcore_PART_SRCS})

target_link_libraries(kdevxdebug
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARY}
    ${KDEVPLATFORM_DEBUGGER_LIBRARY}
)

install(TARGETS kdevxdebug DESTINATION ${PLUGIN_INSTALL_DIR})



########### next target ###############

set(connectiontest_SRCS
    tests/connectiontest.cpp
)

kde4_add_unit_test(connectiontest ${connectiontest_SRCS} ${kdevxdebugcore_PART_SRCS})

target_link_libraries(connectiontest
    ${QT_QTTEST_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARY}
    ${KDEVPLATFORM_TESTSHELL_LIBRARY}
    ${KDEVPLATFORM_DEBUGGER_LIBRARY}
)



########### install files ###############

install(FILES kdevxdebug.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES kdevxdebugui.rc DESTINATION ${DATA_INSTALL_DIR}/kdevxdebug )
