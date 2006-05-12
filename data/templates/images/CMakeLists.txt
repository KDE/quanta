
#add_subdirectory( jpg )
#add_subdirectory( png )
#add_subdirectory( others )

########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/templates/images  FILES  .dirinfo )




#original Makefile.am contents follow:

#SUBDIRS = jpg png others
#
#templatesimagesdir = ${quanta_datadir}/templates/images
#templatesimages_DATA = dirinfo
#
#install-data-hook:
#	@mv $(DESTDIR)$(templatesimagesdir)/dirinfo $(DESTDIR)$(templatesimagesdir)/.dirinfo
