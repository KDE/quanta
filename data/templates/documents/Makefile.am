####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = html others scripts 

EXTRA_DIST = dirinfo 

install-data-local:
	$(mkinstalldirs) ${kde_datadir}/quanta/templates/documents/
	$(INSTALL_DATA) $(srcdir)/dirinfo ${kde_datadir}/quanta/templates/documents/.dirinfo

uninstall-local:
	-rm -f ${kde_datadir}/quanta/templates/documents/.dirinfo

####### kdevelop will overwrite this part!!! (end)############
