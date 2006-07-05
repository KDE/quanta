
#add_subdirectory( scripts )
#add_subdirectory( others )
#add_subdirectory( html )

########### install files ###############

install( FILES  .dirinfo  DESTINATION ${QUANTA_DATA_INSTALL_DIR}/templates/text)




#original Makefile.am contents follow:

#SUBDIRS = scripts others html
#
#templatestextdir = ${quanta_datadir}/templates/text
#templatestext_DATA = dirinfo
#
#install-data-hook:
#	@mv $(DESTDIR)$(templatestextdir)/dirinfo $(DESTDIR)$(templatestextdir)/.dirinfo
