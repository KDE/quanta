####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = php perl javascript 

EXTRA_DIST = dirinfo 

install-data-local:
	$(mkinstalldirs) ${kde_datadir}/quanta/templates/scripts/
	$(INSTALL_DATA) $(srcdir)/dirinfo ${kde_datadir}/quanta/templates/scripts/.dirinfo

uninstall-local:
	-rm -f ${kde_datadir}/quanta/templates/scripts/.dirinfo

####### kdevelop will overwrite this part!!! (end)############
