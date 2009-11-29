project(quanta)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include (MacroLibrary)

add_definitions(-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${quanta_SOURCE_DIR}/../ ${quanta_BINARY_DIR}/../ ${KDE4_INCLUDES})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${quanta_SOURCE_DIR}/cmake/)

set(QUANTA_DATA_INSTALL_DIR ${DATA_INSTALL_DIR}/quanta CACHE STRING "The Quanta data dir")
add_definitions ( -DKDE_DEFAULT_DEBUG_AREA=24000 )

find_package(KDevPlatform REQUIRED)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(quantacore)
add_subdirectory(plugins)
add_subdirectory(data)
add_subdirectory(scripts)


