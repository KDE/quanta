SUBDIRS = scripts others html 

EXTRA_DIST = dirinfo 

templatestextdir = $(kde_datadir)/quanta/templates/text
templatestext_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatestextdir)/dirinfo $(DESTDIR)$(templatestextdir)/.dirinfo
