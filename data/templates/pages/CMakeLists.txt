kde4_header()

#add_subdirectory( php )
#add_subdirectory( html )
#add_subdirectory( docbook )

########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/templates/pages  FILES  .dirinfo )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = php html docbook
#
#templatespagesdir = ${quanta_datadir}/templates/pages
#templatespages_DATA = dirinfo
#
#install-data-hook:
#	@mv $(DESTDIR)$(templatespagesdir)/dirinfo $(DESTDIR)$(templatespagesdir)/.dirinfo
