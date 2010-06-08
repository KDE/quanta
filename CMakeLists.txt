cmake_minimum_required(VERSION 2.6)
project(phpformatter)

SET(CMAKE_COLOR_MAKEFILE ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${phpformatter_SOURCE_DIR}/cmake)

set(KDE_MIN_VERSION "4.2.0")

set(KDE4_BUILD_TESTS "ON" CACHE "BOOL" "Enable building of tests" FORCE )

find_package(KDE4 4.2.0 REQUIRED)

find_package(KDevPlatform 0.9.95 REQUIRED)

include ( KDE4Defaults )

add_definitions (
    ${QT_DEFINITIONS}
    ${KDE4_DEFINITIONS}
)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${KDEVPLATFORM_INCLUDE_DIR} )


set(kdevphpformatter_PART_SRCS formatter.cpp
  formatterplugin.cpp
  formatter.cpp
)

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${QT4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#kde4_add_ui_files(kdevphpformatter_PART_SRCS formatterwidget.ui)

kde4_add_plugin(kdevphpformatter ${kdevphpformatter_PART_SRCS})

target_link_libraries(kdevphpformatter
  ${KDE4_KDEUI_LIBS}
  ${KDEVPLATFORM_INTERFACES_LIBRARIES}
)

install(TARGETS kdevphpformatter DESTINATION ${PLUGIN_INSTALL_DIR})

##next target

set(formattertest_SRCS tests/formattertest.cpp formatter.cpp)
kde4_add_unit_test(formattertest ${formattertest_SRCS})
target_link_libraries( formattertest
    ${QT_QTTEST_LIBRARY} 
    ${KDE4_KDEUI_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
)

########### install files ###############

install(FILES kdevphpformatter.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES 
    phpStylist.php
    DESTINATION ${DATA_INSTALL_DIR}/kdevphpformatter
)