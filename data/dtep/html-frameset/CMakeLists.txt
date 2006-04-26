kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES}  )


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/dtep/html-frameset  FILES  basefont.tag  description.rc  font.tag  frame.tag  frameset.tag  isindex.tag common.tag )

kde4_footer()



#original Makefile.am contents follow:

#quanta_DATA =  basefont.tag  description.rc  font.tag  frame.tag  frameset.tag  isindex.tag common.tag
#quantadir = ${quanta_datadir}/dtep/html-frameset
#
