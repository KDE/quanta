PROJECT(language)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${language_SOURCE_DIR}/cmake)

enable_testing()

add_subdirectory(parser)

find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${QT4_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/parser
)

#Don't error out if the FindXXX are missing for these two
find_package(KDevelop-PG-Qt QUIET)

if(KDEVPGQT_FOUND)
    include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}/parser
        ${KDEVPGQT_INCLUDE_DIR}
    )
else(KDEVPGQT_FOUND)
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/generated
        ${CMAKE_CURRENT_SOURCE_DIR}/parser/generated/kdevelop-pg-qt
    )
endif(KDEVPGQT_FOUND)

set(kdevmllanguagesupport_PART_SRCS
    languagesupport.cpp
    parsejob.cpp
)

kde4_add_plugin(kdevmllanguagesupport ${kdevmllanguagesupport_PART_SRCS})

target_link_libraries(kdevmllanguagesupport
    ${KDE4_KDEUI_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_KTEXTEDITOR_LIBS}
    kdevmlparser
)

install(TARGETS kdevmllanguagesupport DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES kdevmlsupport.desktop DESTINATION ${SERVICES_INSTALL_DIR})

