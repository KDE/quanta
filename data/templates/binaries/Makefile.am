SUBDIRS = images others

templatesbinariesdir = ${quanta_datadir}/templates/binaries
templatesbinaries_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatesbinariesdir)/dirinfo $(DESTDIR)$(templatesbinariesdir)/.dirinfo
