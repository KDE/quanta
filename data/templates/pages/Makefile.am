SUBDIRS = php html docbook

templatespagesdir = ${quanta_datadir}/templates/pages
templatespages_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatespagesdir)/dirinfo $(DESTDIR)$(templatespagesdir)/.dirinfo
