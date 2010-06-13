set(EXECUTESCRIPT_INCLUDE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/executescript)
set(EXECUTEBROWSER_INCLUDE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugins/executebrowser)

add_subdirectory( xdebug )

macro_optional_find_package(QJSON 0.7.1)
macro_log_feature( QJSON_FOUND "QJSON" "QJSON libraries for enabling the Crossfire debugger plugin" 
                               "http://www.gitorious.org/qjson/" FALSE "0.7.1"
                               "The QJSON libraries are needed to build the Crossfire debugger plugin" )
if(QJSON_FOUND)
  add_subdirectory( crossfire )
endif(QJSON_FOUND)