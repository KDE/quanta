
add_subdirectory( config )
add_subdirectory( pics )
add_subdirectory( icons )
add_subdirectory( dtep )
add_subdirectory( toolbars )
add_subdirectory( templates )


########### install files ###############

install_files(${DATA_INSTALL_DIR}/quanta  FILES  tips )




#original Makefile.am contents follow:

#SUBDIRS = pics icons
#
#quantadir = ${quanta_datadir}
#
#quanta_DATA = tips
