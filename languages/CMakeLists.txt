add_subdirectory( sql )

macro_optional_find_package(KDevelop-PG-Qt)
macro_log_feature( KDEVPGQT_FOUND "KDevelop-PG-Qt" "KDevelop-PG-Qt to generate parser for language plugins"
                               "http://gitorious.org/kdevelop/kdevelop-pg-qt" FALSE ""
                               "The KDevelop-PG-Qt parser generator is needed to build the CSS and XML language plugins." )

macro_optional_find_package(Flex)
macro_log_feature( FLEX_FOUND "Flex" "Flex to generate lexer for language plugins"
                               "http://flex.sourceforge.net/" FALSE ""
                               "The Flex lexer generator is needed to build the CSS language plugin." )
if(KDEVPGQT_FOUND)
  if (FLEX_FOUND)
    add_subdirectory( css )
  endif(FLEX_FOUND)

  add_subdirectory( xml )
endif(KDEVPGQT_FOUND)
