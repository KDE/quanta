####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = php html docbook 

EXTRA_DIST = dirinfo 

install-data-local:
	$(mkinstalldirs) ${kde_datadir}/quanta/templates/pages/
	$(INSTALL_DATA) $(srcdir)/dirinfo ${kde_datadir}/quanta/templates/pages/.dirinfo

uninstall-local:
	-rm -f ${kde_datadir}/quanta/templates/pages/.dirinfo

####### kdevelop will overwrite this part!!! (end)############
