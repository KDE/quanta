kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES}  )


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/toolbars/kde-docbook  FILES  kde-docbook-admonitions.toolbar.tgz  	kde-docbook-images.toolbar.tgz  	kde-docbook-list.toolbar.tgz  	kde-docbook-standard.toolbar.tgz  	kde-docbook-tables.toolbar.tgz  	kde-docbook-uielements.toolbar.tgz  	kdexmltools.toolbar.tgz )

kde4_footer()



#original Makefile.am contents follow:

#quantadir = ${quanta_datadir}/toolbars/kde-docbook
#quanta_DATA = kde-docbook-admonitions.toolbar.tgz \
#	kde-docbook-images.toolbar.tgz \
#	kde-docbook-list.toolbar.tgz \
#	kde-docbook-standard.toolbar.tgz \
#	kde-docbook-tables.toolbar.tgz \
#	kde-docbook-uielements.toolbar.tgz \
#	kdexmltools.toolbar.tgz
