SUBDIRS = jpg png others 

EXTRA_DIST = dirinfo 

templatesimagesdir = $(kde_datadir)/quanta/templates/images
templatesimages_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatesimagesdir)/dirinfo $(DESTDIR)$(templatesimagesdir)/.dirinfo
