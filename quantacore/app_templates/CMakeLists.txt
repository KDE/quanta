
set(apptemplate_DIRS
    quanta)
kdevelop_add_app_templates(${apptemplate_DIRS})


########### install files ###############

install( FILES quanta.appwizard  DESTINATION ${DATA_INSTALL_DIR}/kdevplatform/profiles/IDE/quanta/)


