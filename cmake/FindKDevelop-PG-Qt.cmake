#
# Find the KDevelop Parser Generator and set various variables accordingly
#
# KDEVPGQT_DIR and can be set to adjust the directory where the kdevelop-pg-qt
# headers and executable are searched. This should point to the installation
# directory of kdevelop-pg-qt
#
# KDEVPGQT_FOUND - set to TRUE if KDevelop-PG was found FALSE otherwise
#
# KDEVPGQT_INCLUDE_DIR         - include dir of kdevelop-pg, for example /usr/include/kdevelop-pg
#                              can be set by the user to select different include dirs
# KDEVPGQT_EXECUTABLE          - the absolute path to the kdevelop-pg executable
#
# KDEVPGQT_GENERATE(SRC_FILE_VAR OUTPUT language
#                     [NAMESPACE ns] [DEBUG_VISITOR] [TOKEN_TEXT] [DUMP_INFO]
#                     GRAMMARFILE ADDITIONALDEPS)
#     macro to add a custom target for the generation of the parser
#     OUTPUT will be given to kdev-pg as the --output parameter and thus sets the filename prefix
#     NAMESPACE can be given to choose a namespace different from the OUTPUT value
#     DEBUG_VISITOR will run kdevelop-pg with the --debug-visitor argument to generate a simple
#                   visitor that will print debug messages
#     TOKEN_TEXT will run kdevelop-pg with the --token-text argument to generate a simple
#                function that returns a readable name of a token
#     DUMP_INFO will tell kdevelop-pg to dump extra information about symbols, terminals and rules
#               into files in the binary dir
#     Note: The macro only exists when KDEVPG was found
#
# Copyright (c) 2007 Andreas Pakulat <apaku@gmx.de>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if( KDEVPGQT_INCLUDE_DIR AND KDEVPGQT_EXECUTABLE )
    set(KDevelop-PG-Qt_FIND_QUIETLY TRUE)
endif( KDEVPGQT_INCLUDE_DIR AND KDEVPGQT_EXECUTABLE )

if(WIN32)
    file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" _progFiles)
    set(_KDEVPGQT_DIR ${_progFiles}/kdevelop-pg-qt)
endif(WIN32)

if( NOT KDEVPGQT_DIR )
    find_path( _kdevpgqtIncDir kdevelop-pg-qt/kdev-pg-token-stream.h
        PATHS
        ${CMAKE_INSTALL_PREFIX}/include
        ${_KDEVPGQT_DIR}/include
    )
    if( _kdevpgqtIncDir )
        set(KDEVPGQT_INCLUDE_DIR ${_kdevpgqtIncDir}/kdevelop-pg-qt)
    endif( _kdevpgqtIncDir )
    find_program( KDEVPGQT_EXECUTABLE NAMES kdev-pg-qt
        PATHS
        ${CMAKE_INSTALL_PREFIX}/bin
        ${_KDEVPGQT_DIR}/bin
    )

    set(KDEVPGQT_INCLUDE_DIR ${KDEVPGQT_INCLUDE_DIR} CACHE PATH "kdevelop-pg-qt include directory containing the headers")
    set( KDEVPGQT_EXECUTABLE ${KDEVPGQT_EXECUTABLE} CACHE PATH "executable for kdevelop-pg-qt" )
else( NOT KDEVPGQT_DIR )
    find_path( _kdevpgqtIncDir kdevelop-pg-qt/kdev-pg-token-stream.h
        PATHS
        ${CMAKE_INSTALL_PREFIX}/include
        ${_KDEVPGQT_DIR}/include
        ${KDEVPGQT_DIR}/include
    )
    set(KDEVPGQT_INCLUDE_DIR ${_kdevpgIncDir}/kdevelop-pg-qt)
    find_program( KDEVPGQT_EXECUTABLE NAMES kdev-pg-qt
        PATHS
        ${CMAKE_INSTALL_PREFIX}/bin
        ${_KDEVPGQT_DIR}/bin
        ${KDEVPGQT_DIR}/bin
    )
    set(KDEVPGQT_INCLUDE_DIR ${KDEVPGQT_INCLUDE_DIR} CACHE PATH "kdevelop-pg-qt include directory containing the headers")
    set( KDEVPGQT_EXECUTABLE ${KDEVPGQT_EXECUTABLE} CACHE PATH "executable for kdevelop-pg-qt" )
endif( NOT KDEVPGQT_DIR )
if( KDEVPGQT_INCLUDE_DIR
 AND KDEVPGQT_EXECUTABLE)

    if( NOT KDevelop-PG-Qt_FIND_QUIETLY )
        message(STATUS "Found KDevelop-PG-Qt")
    endif( NOT KDevelop-PG-Qt_FIND_QUIETLY )

# if all modules found
    set(KDEVPGQT_FOUND TRUE)

    macro(KDEVPGQT_GENERATE _srcVar _language )
        set(_outputList
            "${CMAKE_CURRENT_BINARY_DIR}/${_language}ast.h"
            "${CMAKE_CURRENT_BINARY_DIR}/${_language}parser.h"
            "${CMAKE_CURRENT_BINARY_DIR}/${_language}parser.cpp"
            "${CMAKE_CURRENT_BINARY_DIR}/${_language}visitor.h"
            "${CMAKE_CURRENT_BINARY_DIR}/${_language}visitor.cpp"
            "${CMAKE_CURRENT_BINARY_DIR}/${_language}defaultvisitor.h"
            "${CMAKE_CURRENT_BINARY_DIR}/${_language}defaultvisitor.cpp"
        )
        set(_depList ${ARGN})
        set(_dbgVisit)
        set(_namespace)
        set(_tokenText)
        set(_dumpInfo)
        set(_beautifulCode)
        set(_conflicts)

        while(1)
        list(GET _depList 0 _arg)
        if( ${_arg} STREQUAL "NAMESPACE" )
            list(GET _depList 1 _namespace)
            list(REMOVE_AT _depList 0 1)
            set(_namespace --namespace=${_namespace})
        elseif( ${_arg} STREQUAL "DEBUG_VISITOR" )
            list(REMOVE_AT _depList 0)
            set(_dbgVisit "--debug-visitor")
            set(_outputList ${_outputList}
                "${CMAKE_CURRENT_BINARY_DIR}/${_language}debugvisitor.h"
            )
        elseif( ${_arg} STREQUAL "TOKEN_TEXT" )
            list(REMOVE_AT _depList 0)
            set(_tokenText "--token-text")
            set(_outputList ${_outputList}
                "${CMAKE_CURRENT_BINARY_DIR}/${_language}tokentext.h"
            )
        elseif( ${_arg} STREQUAL "DUMP_INFO" )
            list(REMOVE_AT _depList 0)
            set(_dumpInfo --terminals --symbols --rules)
            set(_outputList ${_outputList}
                "${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-terminals"
                "${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-symbols"
                "${CMAKE_CURRENT_BINARY_DIR}/kdev-pg-rules"
            )
        elseif( ${_arg} STREQUAL "BEAUTIFUL_CODE" )
            list(REMOVE_AT _depList 0)
            set(_beautifulCode --beautiful-code)
        elseif( ${_arg} STREQUAL "STRICT_CONFLICTS" )
            list(REMOVE_AT _depList 0)
            set(_conflicts --strict-conflicts)
        elseif( ${_arg} STREQUAL "IGNORE_CONFLICTS" )
            list(REMOVE_AT _depList 0)
            set(_conflicts --ignore-conflicts)
        else( ${_arg} STREQUAL "IGNORE_CONFLICTS" )
            break()
        endif( ${_arg} STREQUAL "NAMESPACE" )
        endwhile(1)

        list(GET _depList 0 _grammarFile)
        list(REMOVE_AT _depList 0)
        if(NOT _grammarFile)
            message(ERROR "No grammar file given to KDEVPGQT_GENERATE macro")
        endif(NOT _grammarFile)
        add_custom_command(
            OUTPUT  ${_outputList}
            MAIN_DEPENDENCY "${_grammarFile}"
            DEPENDS ${_depList}
            COMMAND ${KDEVPGQT_EXECUTABLE}
            ARGS    --output=${_language} ${_namespace}
                    ${_dbgVisit} ${_dumpInfo} ${_beautifulCode} ${_conflicts} "${_grammarFile}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        )

        set( ${_srcVar}
            "${CMAKE_CURRENT_BINARY_DIR}/${_language}parser.cpp"
            "${CMAKE_CURRENT_BINARY_DIR}/${_language}visitor.cpp"
            "${CMAKE_CURRENT_BINARY_DIR}/${_language}defaultvisitor.cpp" )
    endmacro(KDEVPGQT_GENERATE)


else( KDEVPGQT_INCLUDE_DIR
 AND KDEVPGQT_EXECUTABLE)

    if( KDevelop-PG-Qt_FIND_REQUIRED)
        message(FATAL_ERROR "Couldn't find KDevelop-PG-Qt.")
    else( KDevelop-PG-Qt_FIND_REQUIRED)
        message(STATUS "Couldn't find KDevelop-PG-Qt.")
    endif( KDevelop-PG-Qt_FIND_REQUIRED)
    message(STATUS "You can set KDEVPGQT_DIR to help cmake find KDevelop-PG-Qt")
    set(KDEVPGQT_FOUND FALSE)

endif( KDEVPGQT_INCLUDE_DIR
 AND KDEVPGQT_EXECUTABLE)

if(KDEVPGQT_FOUND)
message(STATUS "Using kdevelop-pg-qt include dir: ${KDEVPGQT_INCLUDE_DIR}")
message(STATUS "Using kdevelop-pg-qt executable: ${KDEVPGQT_EXECUTABLE}")
endif(KDEVPGQT_FOUND)


