kde4_header()

#add_subdirectory( html )
#add_subdirectory( others )
#add_subdirectory( scripts )


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/templates/document  FILES  .dirinfo )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = html others scripts 
#
#templatesdocumentsdir = ${quanta_datadir}/templates/documents
#templatesdocuments_DATA = dirinfo
#
#install-data-hook:
#	@mv $(DESTDIR)$(templatesdocumentsdir)/dirinfo $(DESTDIR)$(templatesdocumentsdir)/.dirinfo
