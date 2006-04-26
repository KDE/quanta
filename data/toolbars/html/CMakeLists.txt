kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES}  )


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/toolbars/html  FILES  forms.toolbar.tgz lists.toolbar.tgz other.toolbar.tgz standard.toolbar.tgz tables.toolbar.tgz style.toolbar.tgz )

kde4_footer()



#original Makefile.am contents follow:

#htmltemplatedir = ${quanta_datadir}/toolbars/html
#htmltemplate_DATA = forms.toolbar.tgz lists.toolbar.tgz other.toolbar.tgz standard.toolbar.tgz tables.toolbar.tgz style.toolbar.tgz
