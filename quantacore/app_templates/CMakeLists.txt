kde4_header()

set(apptemplate_DIRS
    quanta)
kdevelop_add_app_templates(${apptemplate_DIRS})


########### install files ###############

install_files(${DATA_INSTALL_DIR}/kdevelop/profiles/IDE/quanta/ FILES quanta.appwizard )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = quanta
#profilesdir = $(kde_datadir)/kdevelop/profiles/IDE/Quanta/
#profiles_DATA = quanta.appwizard
