kde4_header()

#add_subdirectory( scripts )
#add_subdirectory( others )
#add_subdirectory( html )

########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/templates/text  FILES  .dirinfo )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = scripts others html
#
#templatestextdir = ${quanta_datadir}/templates/text
#templatestext_DATA = dirinfo
#
#install-data-hook:
#	@mv $(DESTDIR)$(templatestextdir)/dirinfo $(DESTDIR)$(templatestextdir)/.dirinfo
