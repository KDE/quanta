SUBDIRS = images others 

EXTRA_DIST = dirinfo 

templatesbinariesdir = $(kde_datadir)/quanta/templates/binaries
templatesbinaries_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatesbinariesdir)/dirinfo $(DESTDIR)$(templatesbinariesdir)/.dirinfo
