
#add_subdirectory( php )
#add_subdirectory( perl )
#add_subdirectory( javascript )

########### install files ###############

install( FILES  .dirinfo  DESTINATION ${QUANTA_DATA_INSTALL_DIR}/templates/scripts)




#original Makefile.am contents follow:

#SUBDIRS = php perl javascript
#
#templatesscriptsdir = ${quanta_datadir}/templates/scripts
#templatesscripts_DATA = dirinfo
#
#install-data-hook:
#	@mv $(DESTDIR)$(templatesscriptsdir)/dirinfo $(DESTDIR)$(templatesscriptsdir)/.dirinfo
