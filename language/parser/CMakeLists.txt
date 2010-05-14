set(KDEVSGMLPARSER_LIB_VERSION 1.0.0)
set(KDEVSGMLPARSER_LIB_SOVERSION 1)

enable_testing()

include_directories(
    ${KDEVPLATFORM_INCLUDE_DIR}
    ${KDE4_INCLUDES}
    ${QT4_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(KDevelop-PG-Qt REQUIRED)

include_directories(
    ${KDEVPGQT_INCLUDE_DIR}
)
kdevpgqt_generate(_kdevpgList sgml NAMESPACE Xml DEBUG_VISITOR
    "${CMAKE_CURRENT_SOURCE_DIR}/sgml.g"
    "${CMAKE_CURRENT_SOURCE_DIR}/sgmltokenizer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/dtdtokenizer.h"
)
set( parser_SRCS
    ${_kdevpgList}
)

set(parser_STAT_SRCS
    tokenizer.cpp
    sgmltokenizer.cpp
    dtdtokenizer.cpp
    parsesession.cpp
    editorintegrator.cpp
    idtdhelper.cpp
    )

### next target
kde4_add_library( kdevsgmlparser SHARED ${parser_SRCS} ${parser_STAT_SRCS} )
target_link_libraries( kdevsgmlparser
    ${KDE4_KDECORE_LIBS}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${KDE4_KDEUI_LIBS}
)
install(TARGETS kdevsgmlparser DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})

### next target
kde4_add_executable(sgmlparser tests/main.cpp ${parser_SRCS} ${parser_STAT_SRCS} )
target_link_libraries(sgmlparser ${QT_QTCORE_LIBRARY} ${KDEVPLATFORM_LANGUAGE_LIBRARIES})

### next target
set(parsertest_SRCS tests/parsertest.cpp)
kde4_add_unit_test(sgmlparsertest ${parsertest_SRCS})
target_link_libraries(sgmlparsertest kdevsgmlparser ${QT_QTTEST_LIBRARY})

### next target
set(sgmltokenizertest_SRCS tests/sgmltokenizertest.cpp)
kde4_add_unit_test(sgmltokenizertest ${sgmltokenizertest_SRCS})
target_link_libraries(sgmltokenizertest kdevsgmlparser ${QT_QTTEST_LIBRARY})
