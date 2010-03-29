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

#Don't error out if the FindXXX are missing for these two
find_package(KDevelop-PG-Qt QUIET)

if(KDEVPGQT_FOUND)
    include_directories(
        ${KDEVPGQT_INCLUDE_DIR}
    )
    kdevpgqt_generate(_kdevpgList sgml NAMESPACE Xml DEBUG_VISITOR
        "${CMAKE_CURRENT_SOURCE_DIR}/sgml.g"
        "${CMAKE_CURRENT_SOURCE_DIR}/sgmllexer.h"
    )
    set( parser_SRCS
       ${_kdevpgList}
    )
else(KDEVPGQT_FOUND)
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/generated
        ${CMAKE_CURRENT_SOURCE_DIR}/generated/kdevelop-pg-qt
    )
    message(STATUS "Assuming existence of generated parser files")
    set(parser_SRCS
        generated/sgmlparser.cpp
        generated/sgmlvisitor.cpp
        generated/sgmldefaultvisitor.cpp )
endif(KDEVPGQT_FOUND)


set(parser_STAT_SRCS
    sgmllexer.cpp
    parsesession.cpp
    editorintegrator.cpp
    stringreflist.cpp
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
set(parsertest_SRCS tests/parsertest.cpp)
kde4_add_unit_test(parsertest ${parsertest_SRCS})
target_link_libraries(parsertest kdevsgmlparser ${QT_QTTEST_LIBRARY})

### next target
set(sgmllexertest_SRCS tests/sgmllexertest.cpp stringreflist.cpp)
kde4_add_unit_test(sgmllexertest ${sgmllexertest_SRCS})
target_link_libraries(sgmllexertest kdevsgmlparser ${QT_QTTEST_LIBRARY})

#Does not work?
add_custom_target( copy-generated
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/sgmlparser.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/generated/sgmlparser.cpp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/sgmlparser.h" "${CMAKE_CURRENT_SOURCE_DIR}/generated/sgmlparser.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/sgmlast.h" "${CMAKE_CURRENT_SOURCE_DIR}/generated/sgmlast.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/sgmlvisitor.h" "${CMAKE_CURRENT_SOURCE_DIR}/generated/sgmlvisitor.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/sgmlvisitor.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/generated/sgmlvisitor.cpp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/sgmldefaultvisitor.h" "${CMAKE_CURRENT_SOURCE_DIR}/generated/sgmldefaultvisitor.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/sgmldefaultvisitor.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/generated/sgmldefaultvisitor.cpp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/sgmldebugvisitor.h" "${CMAKE_CURRENT_SOURCE_DIR}/generated/sgmldebugvisitor.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/sgmlast.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/sgmlparser.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/sgmlparser.cpp"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/sgmlvisitor.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/sgmlvisitor.cpp"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/sgmldefaultvisitor.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/sgmldefaultvisitor.cpp"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/sgmldebugvisitor.h"
)

