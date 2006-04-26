kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES}  )


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/dtep/empty  FILES  description.rc  )

kde4_footer()



#original Makefile.am contents follow:

#quantadir = ${quanta_datadir}/dtep/empty
#
#quanta_DATA = description.rc 
#
