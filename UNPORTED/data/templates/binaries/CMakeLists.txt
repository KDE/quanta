
#add_subdirectory( images )
#add_subdirectory( others )


########### install files ###############

install( FILES  .dirinfo  DESTINATION ${QUANTA_DATA_INSTALL_DIR}/templates/binaries)




#original Makefile.am contents follow:

#SUBDIRS = images others
#
#templatesbinariesdir = ${quanta_datadir}/templates/binaries
#templatesbinaries_DATA = dirinfo
#
#install-data-hook:
#	@mv $(DESTDIR)$(templatesbinariesdir)/dirinfo $(DESTDIR)$(templatesbinariesdir)/.dirinfo
