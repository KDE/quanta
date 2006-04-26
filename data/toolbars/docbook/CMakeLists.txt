kde4_header()


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/toolbars/docbook  FILES  xmltools.toolbar.tgz  docbook_code.toolbar.tgz  docbook_ui.toolbar.tgz  docbook_xml.toolbar.tgz  )

kde4_footer()



#original Makefile.am contents follow:

#quantadir = ${quanta_datadir}/toolbars/docbook
#quanta_DATA = xmltools.toolbar.tgz \
#docbook_code.toolbar.tgz \
#docbook_ui.toolbar.tgz \
#docbook_xml.toolbar.tgz 
