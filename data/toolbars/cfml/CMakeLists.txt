kde4_header()


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/toolbars/cfml  FILES  fonts.toolbar.tgz forms.toolbar.tgz lists.toolbar.tgz standard.toolbar.tgz tables.toolbar.tgz cfml.toolbar.tgz )

kde4_footer()



#original Makefile.am contents follow:

#quanta_DATA = fonts.toolbar.tgz forms.toolbar.tgz lists.toolbar.tgz standard.toolbar.tgz tables.toolbar.tgz cfml.toolbar.tgz
#quantadir = ${quanta_datadir}/toolbars/cfml
