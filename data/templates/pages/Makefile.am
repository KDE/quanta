SUBDIRS = php html docbook 

EXTRA_DIST = dirinfo 

templatespagesdir = $(kde_datadir)/quanta/templates/pages
templatespages_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatespagesdir)/dirinfo $(DESTDIR)$(templatespagesdir)/.dirinfo
