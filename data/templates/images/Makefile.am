SUBDIRS = jpg png others

templatesimagesdir = $(kde_datadir)/quanta/templates/images
templatesimages_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatesimagesdir)/dirinfo $(DESTDIR)$(templatesimagesdir)/.dirinfo
