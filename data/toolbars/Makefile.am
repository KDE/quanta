####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = docbook.toolbar.tgz kde_docbook.toolbar.tgz 

install-data-local:
	$(mkinstalldirs) ${kde_datadir}/quanta/toolbars/
	$(INSTALL_DATA) $(srcdir)/docbook.toolbar.tgz ${kde_datadir}/quanta/toolbars/docbook.toolbar.tgz
	$(mkinstalldirs) ${kde_datadir}/quanta/toolbars/
	$(INSTALL_DATA) $(srcdir)/kde_docbook.toolbar.tgz ${kde_datadir}/quanta/toolbars/kde_docbook.toolbar.tgz

uninstall-local:
	-rm -f ${kde_datadir}/quanta/toolbars/docbook.toolbar.tgz
	-rm -f ${kde_datadir}/quanta/toolbars/kde_docbook.toolbar.tgz

####### kdevelop will overwrite this part!!! (end)############
