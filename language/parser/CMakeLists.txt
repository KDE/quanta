set(KDEVMLPARSER_LIB_VERSION 1.0.0)
set(KDEVMLPARSER_LIB_SOVERSION 1)

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
    kdevpgqt_generate(_kdevpgList ml NAMESPACE Ml DEBUG_VISITOR
        "${CMAKE_CURRENT_SOURCE_DIR}/ml.g"
        "${CMAKE_CURRENT_SOURCE_DIR}/mllexer.h"
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
        generated/mlparser.cpp
        generated/mlvisitor.cpp
        generated/mldefaultvisitor.cpp )
endif(KDEVPGQT_FOUND)


set(parser_STAT_SRCS
    mllexer.cpp
    parsesession.cpp
    editorintegrator.cpp
    )

### next target
kde4_add_library( kdevmlparser SHARED ${parser_SRCS} ${parser_STAT_SRCS} )
target_link_libraries( kdevmlparser
    ${KDE4_KDECORE_LIBS}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${KDE4_KDEUI_LIBS}   
)
install(TARGETS kdevmlparser DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})

### next target
set(parsertest_SRCS tests/parsertest.cpp)
kde4_add_unit_test(parsertest ${parsertest_SRCS})
target_link_libraries(parsertest kdevmlparser ${QT_QTTEST_LIBRARY})

### next target
set(mllexertest_SRCS tests/mllexertest.cpp)
kde4_add_unit_test(mllexertest ${mllexertest_SRCS})
target_link_libraries(mllexertest kdevmlparser ${QT_QTTEST_LIBRARY})

#Does not work?
add_custom_target( copy-generated
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/mlparser.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/generated/mlparser.cpp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/mlparser.h" "${CMAKE_CURRENT_SOURCE_DIR}/generated/mlparser.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/mlast.h" "${CMAKE_CURRENT_SOURCE_DIR}/generated/mlast.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/mlvisitor.h" "${CMAKE_CURRENT_SOURCE_DIR}/generated/mlvisitor.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/mlvisitor.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/generated/mlvisitor.cpp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/mldefaultvisitor.h" "${CMAKE_CURRENT_SOURCE_DIR}/generated/mldefaultvisitor.h"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/mldefaultvisitor.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/generated/mldefaultvisitor.cpp"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/mldebugvisitor.h" "${CMAKE_CURRENT_SOURCE_DIR}/generated/mldebugvisitor.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/mlast.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/mlparser.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/mlparser.cpp"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/mlvisitor.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/mlvisitor.cpp"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/mldefaultvisitor.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/mldefaultvisitor.cpp"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/mldebugvisitor.h"
)

