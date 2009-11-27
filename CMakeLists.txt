project(xml)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${xml_SOURCE_DIR}/cmake)

set(KDE_MIN_VERSION "4.2.0")

find_package(KDE4 4.2.0 REQUIRED)
find_package(KDevPlatform 0.9.95 REQUIRED)

include (KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DHAVE_CONFIG_H=1)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${KDEVPLATFORM_INCLUDE_DIR} )

#add_subdirectory(catalog)

#This is not complete and probably completely the wrong approach.
#It does compile however and have some / limited functionality.
# a good candidate to comment out
add_subdirectory(document)

add_subdirectory(formatter)

add_subdirectory(validator)
