####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = jpg png others buttons banners 

EXTRA_DIST = dirinfo 

install-data-local:
	$(mkinstalldirs) ${kde_datadir}/quanta/templates/images/
	$(INSTALL_DATA) $(srcdir)/dirinfo ${kde_datadir}/quanta/templates/images/.dirinfo

uninstall-local:
	-rm -f ${kde_datadir}/quanta/templates/images/.dirinfo

####### kdevelop will overwrite this part!!! (end)############

