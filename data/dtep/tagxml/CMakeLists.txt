kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES}  )


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/dtep/tagxml  FILES  childstop.tag spacer.tag attr.tag location.tag tag.tag nonparams.tag vpl.tag description.rc  )

kde4_footer()



#original Makefile.am contents follow:

#quanta_DATA = childstop.tag spacer.tag attr.tag location.tag tag.tag nonparams.tag vpl.tag description.rc 
#quantadir = ${quanta_datadir}/dtep/tagxml
#
