####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = fonts.toolbar.tgz fonts.toolbar.tgz 

install-data-local:
	$(mkinstalldirs) ${kde_datadir}/quanta/toolbars/html/
	$(INSTALL_DATA) $(srcdir)/fonts.toolbar.tgz ${kde_datadir}/quanta/toolbars/html/fonts.toolbar.tgz
	$(mkinstalldirs) ${kde_datadir}/quanta/toolbars/xml/
	$(INSTALL_DATA) $(srcdir)/fonts.toolbar.tgz ${kde_datadir}/quanta/toolbars/xml/fonts.toolbar.tgz

uninstall-local:
	-rm -f ${kde_datadir}/quanta/toolbars/html/fonts.toolbar.tgz
	-rm -f ${kde_datadir}/quanta/toolbars/xml/fonts.toolbar.tgz

####### kdevelop will overwrite this part!!! (end)############
