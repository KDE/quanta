SUBDIRS = html others scripts 

templatesdocumentsdir = $(kde_datadir)/quanta/templates/documents
templatesdocuments_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatesdocumentsdir)/dirinfo $(DESTDIR)$(templatesdocumentsdir)/.dirinfo
