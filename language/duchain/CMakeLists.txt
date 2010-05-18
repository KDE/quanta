
add_subdirectory(navigation)

include_directories(
    ${xml_SOURCE_DIR}/language/parser
    ${xml_BINARY_DIR}/language/parser
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(KDevelop-PG-Qt REQUIRED)

include_directories(
    ${KDEVPGQT_INCLUDE_DIR}
)


set( duchain_SRCS
    elementdeclaration.cpp
    declarationbuilder.cpp
    contextbuilder.cpp
    includebuilder.cpp
    xsdcontextbuilder.cpp
    xsddeclarationbuilder.cpp
    navigation/navigationwidget.cpp
)


kde4_add_library( kdevsgmlduchain SHARED ${duchain_SRCS} )
target_link_libraries(kdevsgmlduchain
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDEVPLATFORM_SHELL_LIBRARIES}
    ${KDEVPLATFORM_PROJECT_LIBRARIES}
    kdevsgmlparser
    kdevxmlcatalog
)

install(TARGETS kdevsgmlduchain DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})

### next target
set(duchaintest_SRCS tests/duchain.cpp tests/duchaintestbase.cpp)
kde4_add_unit_test(duchaintest ${duchaintest_SRCS})
target_link_libraries(duchaintest
    ${QT_QTTEST_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDEVPLATFORM_SHELL_LIBRARIES}
    ${KDEVPLATFORM_PROJECT_LIBRARIES}
    kdevsgmlduchain
    kdevsgmlparser
    kdevxmlcatalog
)

get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
get_property(ln_dirs DIRECTORY PROPERTY LINK_DIRECTORIES)
message(STATUS "include_directories: ${inc_dirs}")
message(STATUS "link_directories: ${ln_dirs}")

