
kdevpgqt_generate(_kdevpgList css NAMESPACE Css DEBUG_VISITOR TOKEN_TEXT
    "${css_SOURCE_DIR}/parser/css.g"
    "${css_SOURCE_DIR}/parser/tokenizer.h"
)
set( parser_SRCS
    ${_kdevpgList}
)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

find_package(Flex REQUIRED)

# Copy tokenizer.ll to the builddir, so that flex doesn't write out
# absolute paths in the generated file when we pass them as arguments.
# In short, I don't want stuff like
# '#line 2 "/home/kde/build/.../tokenizer.cpp" in SVN.
add_custom_command(
    OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.flex"
    MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.flex"
    COMMAND ${CMAKE_COMMAND}  ARGS -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.flex"
            "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.flex"
)
set_source_files_properties(
    "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.flex"
    GENERATED
)
# Add command to generate the lexer.
add_custom_command(
    OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.cpp"
    MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/tokenizer.flex"
    COMMAND ${FLEX_EXECUTABLE}
    ARGS    -o"tokenizer.cpp"
            "tokenizer.flex"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)



set(parser_STAT_SRCS
    ${CMAKE_CURRENT_BINARY_DIR}/tokenizer.cpp
    parsesession.cpp
    editorintegrator.cpp
    htmlparser.cpp
    )

### next target
kde4_add_library( kdev4cssparser SHARED ${parser_SRCS} ${parser_STAT_SRCS} )
target_link_libraries( kdev4cssparser
    ${KDE4_KDECORE_LIBS}
    ${KDEVPLATFORM_LANGUAGE_LIBRARIES}
    ${KDE4_KTEXTEDITOR_LIBS}
)
install(TARGETS kdev4cssparser DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})

### next target
kde4_add_executable( css-parser main.cpp ${parser_SRCS} ${parser_STAT_SRCS} )
target_link_libraries(css-parser
    ${QT_QTCORE_LIBRARY} ${KDEVPLATFORM_LANGUAGE_LIBRARIES} ${KDEVPLATFORM_TESTS_LIBRARIES})

### next target
set(parsertest_SRCS test/parsertest.cpp)
kde4_add_unit_test(parsertest ${parsertest_SRCS})
target_link_libraries(parsertest kdev4cssparser ${QT_QTTEST_LIBRARY} ${KDE4_KDECORE_LIBS} ${KDEVPLATFORM_LANGUAGE_LIBRARIES})
