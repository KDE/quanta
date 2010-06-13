add_subdirectory( sql )

find_package(KDevelop-PG-Qt REQUIRED)

macro_optional_find_package(KDevelop-PG-Qt)
macro_log_feature( KDEVPGQT_FOUND "KDevelop-PG-Qt" "KDevelop-PG-Qt to generate parser for language plugins"
                               "http://gitorious.org/kdevelop/kdevelop-pg-qt" FALSE ""
                               "The KDevelop-PG-Qt parser generator is needed to build the CSS and XML language plugins." )
if(KDEVPGQT_FOUND)
  add_subdirectory( css )
  add_subdirectory( xml )
endif(KDEVPGQT_FOUND)
