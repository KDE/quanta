SUBDIRS = html others scripts 

templatesdocumentsdir = ${quanta_datadir}/templates/documents
templatesdocuments_DATA = dirinfo

install-data-hook:
	@mv $(DESTDIR)$(templatesdocumentsdir)/dirinfo $(DESTDIR)$(templatesdocumentsdir)/.dirinfo
