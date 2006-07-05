

add_subdirectory( html )
add_subdirectory( xml )
add_subdirectory( wml-1-2 )
add_subdirectory( tagxml )
add_subdirectory( schema )
add_subdirectory( cfml )
add_subdirectory( kde-docbook )
add_subdirectory( docbook )
add_subdirectory( css )


########### install files ###############

install( FILES  debug.toolbar.tgz  DESTINATION ${QUANTA_DATA_INSTALL_DIR}/toolbars)




#original Makefile.am contents follow:

#SUBDIRS = html xml wml-1-2 tagxml schema cfml kde-docbook docbook css
#
#toolbarsdir = ${quanta_datadir}/toolbars
#toolbars_DATA = debug.toolbar.tgz
