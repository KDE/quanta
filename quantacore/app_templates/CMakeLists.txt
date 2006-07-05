
set(apptemplate_DIRS
    quanta)
kdevelop_add_app_templates(${apptemplate_DIRS})


########### install files ###############

install( FILES quanta.appwizard  DESTINATION ${DATA_INSTALL_DIR}/kdevelop/profiles/IDE/quanta/)




#original Makefile.am contents follow:

#SUBDIRS = quanta
#profilesdir = $(kde_datadir)/kdevelop/profiles/IDE/Quanta/
#profiles_DATA = quanta.appwizard
