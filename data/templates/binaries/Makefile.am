####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = images others 

EXTRA_DIST = dirinfo 

install-data-local:
	$(mkinstalldirs) ${kde_datadir}/quanta/templates/binaries/
	$(INSTALL_DATA) $(srcdir)/dirinfo ${kde_datadir}/quanta/templates/binaries/.dirinfo

uninstall-local:
	-rm -f ${kde_datadir}/quanta/templates/binaries/.dirinfo

####### kdevelop will overwrite this part!!! (end)############

