SUBDIRS = php perl javascript

templatesscriptsdir = ${quanta_datadir}/templates/scripts
templatesscripts_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatesscriptsdir)/dirinfo $(DESTDIR)$(templatesscriptsdir)/.dirinfo
