SUBDIRS = scripts others html

templatestextdir = ${quanta_datadir}/templates/text
templatestext_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatestextdir)/dirinfo $(DESTDIR)$(templatestextdir)/.dirinfo
