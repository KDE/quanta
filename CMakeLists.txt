project(xdebug)

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=9046 )


enable_testing()
find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)

find_path( EXECUTESCRIPT_INCLUDE_DIR iexecutescriptplugin.h
 PATH_SUFFIXES kdevexecutescript )
 
include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EXECUTESCRIPT_INCLUDE_DIR}
)

########### next target ###############

set(kdevxdebugcore_PART_SRCS
    connection.cpp
    debugsession.cpp
    debugjob.cpp
    framestackmodel.cpp
    breakpointcontroller.cpp
)

set(kdevxdebug_PART_SRCS
    xdebugplugin.cpp
    launchconfig.cpp
)

kde4_add_plugin(kdevxdebug ${kdevxdebug_PART_SRCS} ${kdevxdebugcore_PART_SRCS})

target_link_libraries(kdevxdebug
    ${KDEVPLATFORM_INTERFACES_LIBRARY}
    ${KDEVPLATFORM_DEBUGGER_LIBRARY}
    ${KDEVPLATFORM_OUTPUTVIEW_LIBRARY}
    ${KDEVPLATFORM_UTIL_LIBRARY}
)

install(TARGETS kdevxdebug DESTINATION ${PLUGIN_INSTALL_DIR})



########### next target ###############

set(connectiontest_SRCS
    tests/connectiontest.cpp
)

kde4_add_unit_test(connectiontest ${connectiontest_SRCS} ${kdevxdebugcore_PART_SRCS})

target_link_libraries(connectiontest
    ${QT_QTTEST_LIBRARY}
    ${KDEVPLATFORM_INTERFACES_LIBRARY}
    ${KDEVPLATFORM_TESTSHELL_LIBRARY}
    ${KDEVPLATFORM_DEBUGGER_LIBRARY}
    ${KDEVPLATFORM_TESTS_LIBRARY}
)



########### install files ###############

install(FILES kdevxdebug.desktop DESTINATION ${SERVICES_INSTALL_DIR})
