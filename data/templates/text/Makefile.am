####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = scripts others html 

EXTRA_DIST = dirinfo 

install-data-local:
	$(mkinstalldirs) ${kde_datadir}/quanta/templates/text/
	$(INSTALL_DATA) $(srcdir)/dirinfo ${kde_datadir}/quanta/templates/text/.dirinfo

uninstall-local:
	-rm -f ${kde_datadir}/quanta/templates/text/.dirinfo

####### kdevelop will overwrite this part!!! (end)############
