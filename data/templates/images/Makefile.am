SUBDIRS = jpg png others

templatesimagesdir = ${quanta_datadir}/templates/images
templatesimages_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatesimagesdir)/dirinfo $(DESTDIR)$(templatesimagesdir)/.dirinfo
