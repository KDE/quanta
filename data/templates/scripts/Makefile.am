SUBDIRS = php perl javascript

templatesscriptsdir = $(kde_datadir)/quanta/templates/scripts
templatesscripts_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatesscriptsdir)/dirinfo $(DESTDIR)$(templatesscriptsdir)/.dirinfo
