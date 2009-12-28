project(quanta)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include (MacroLibrary)

add_definitions(-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${quanta_SOURCE_DIR}/../ ${quanta_BINARY_DIR}/../ ${KDE4_INCLUDES})

set(CMAKE_MODULE_PATH ${quanta_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH} )

set(QUANTA_DATA_INSTALL_DIR ${DATA_INSTALL_DIR}/quanta CACHE STRING "The Quanta data dir")
add_definitions ( -DKDE_DEFAULT_DEBUG_AREA=24000 )

if( extragear-sdk_SOURCE_DIR AND ( ${extragear-sdk_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR}  ) )
    set( KDEVPLATFORM_FOUND TRUE )
    include_directories( ${CMAKE_SOURCE_DIR}/kdevplatform ${CMAKE_SOURCE_DIR}/kdevplatform/plugins )
    set( KDEVPLATFORM_INTERFACES_LIBRARIES kdevplatforminterfaces )
    set( KDEVPLATFORM_SHELL_LIBRARIES kdevplatformshell )
    set( KDEVPLATFORM_TESTS_LIBRARIES kdevplatformtests )
    set( KDEVPLATFORM_UTIL_LIBRARIES kdevplatformutil )
    set( KDEVPLATFORM_PROJECT_LIBRARIES kdevplatformproject )
    set( KDEVPLATFORM_OUTPUTVIEW_LIBRARIES kdevplatformoutputview )
    set( KDEVPLATFORM_LANGUAGE_LIBRARIES kdevplatformlanguage )
    set( KDEVPLATFORM_VCS_LIBRARIES kdevplatformvcs )
    set( KDEVPLATFORM_DEBUGGER_LIBRARIES kdevplatformdebugger )
    set( KDEVPLATFORM_SUBLIME_LIBRARIES sublime )
    include( ${CMAKE_SOURCE_DIR}/kdevplatform/cmake/modules/KDevPlatformMacros.cmake )
else( extragear-sdk_SOURCE_DIR AND ( ${extragear-sdk_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR}  ) )

find_package(KDevPlatform 0.9.97 REQUIRED)
include_directories(${KDEVPLATFORM_INCLUDE_DIR})

endif( extragear-sdk_SOURCE_DIR AND ( ${extragear-sdk_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR}  ) )

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(quantacore)
add_subdirectory(plugins)
add_subdirectory(data)
add_subdirectory(scripts)


