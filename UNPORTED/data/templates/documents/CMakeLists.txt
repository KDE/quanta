
#add_subdirectory( html )
#add_subdirectory( others )
#add_subdirectory( scripts )


########### install files ###############

install( FILES  .dirinfo  DESTINATION ${QUANTA_DATA_INSTALL_DIR}/templates/document)




#original Makefile.am contents follow:

#SUBDIRS = html others scripts 
#
#templatesdocumentsdir = ${quanta_datadir}/templates/documents
#templatesdocuments_DATA = dirinfo
#
#install-data-hook:
#	@mv $(DESTDIR)$(templatesdocumentsdir)/dirinfo $(DESTDIR)$(templatesdocumentsdir)/.dirinfo
