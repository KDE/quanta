SUBDIRS = php perl javascript 

EXTRA_DIST = dirinfo 

templatesscriptsdir = $(kde_datadir)/quanta/templates/scripts
templatesscripts_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatesscriptsdir)/dirinfo $(DESTDIR)$(templatesscriptsdir)/.dirinfo
