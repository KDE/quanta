project(crossfire)

enable_testing()
find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)
find_package(QJSON REQUIRED)

find_path( EXECUTEBROWSER_INCLUDE_DIR iexecutebrowserplugin.h
 PATH_SUFFIXES kdevexecutebrowser )

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EXECUTEBROWSER_INCLUDE_DIR}
    # we want to have explicit qjson includes since it's headers are too generically named
    ${QJSON_INCLUDE_DIR}/..
)

set(kdevcrossfirecore_PART_SRCS
    connection.cpp
    debugsession.cpp
    debugjob.cpp
    breakpointcontroller.cpp
    framestackmodel.cpp
    variablecontroller.cpp
    variable.cpp
    stringhelpers.cpp
)

########### next target ###############

set(kdevcrossfire_PART_SRCS
    crossfireplugin.cpp
    launchconfig.cpp
    launchconfigurationpage.cpp
)
kde4_add_plugin(kdevcrossfire ${kdevcrossfire_PART_SRCS} ${kdevcrossfirecore_PART_SRCS})
target_link_libraries(kdevcrossfire
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_DEBUGGER_LIBRARIES}
    ${KDEVPLATFORM_OUTPUTVIEW_LIBRARIES}
    ${KDEVPLATFORM_UTIL_LIBRARIES}
    ${QJSON_LIBRARIES}
)
install(TARGETS kdevcrossfire DESTINATION ${PLUGIN_INSTALL_DIR})


########### next target ###############
set(connectiontest_SRCS
    tests/connectiontest.cpp
)
kde4_add_unit_test(connectiontest ${connectiontest_SRCS} ${kdevcrossfirecore_PART_SRCS})
target_link_libraries(connectiontest
    ${QT_QTTEST_LIBRARY}
    ${KDEVPLATFORM_DEBUGGER_LIBRARIES}
    ${QJSON_LIBRARIES}
)

########### next target ###############
set(sessiontest_SRCS
    tests/sessiontest.cpp
)
kde4_add_unit_test(sessiontest ${sessiontest_SRCS} ${kdevcrossfirecore_PART_SRCS})
target_link_libraries(sessiontest
    ${QT_QTTEST_LIBRARY}
    ${KDEVPLATFORM_INTERFACE_LIBRARIES}
    ${KDEVPLATFORM_DEBUGGER_LIBRARIES}
    ${QJSON_LIBRARIES}
    ${KDEVPLATFORM_TESTSHELL_LIBRARIES}
    ${KDEVPLATFORM_TESTS_LIBRARIES}
)


########### install files ###############
install(FILES kdevcrossfire.desktop DESTINATION ${SERVICES_INSTALL_DIR})
