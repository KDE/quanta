####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = tagdata.rc 

install-data-local:
	$(mkinstalldirs) $(kde_datadir)/quanta/
	$(mkinstalldirs) $(kde_datadir)/quanta/doc
	$(mkinstalldirs) $(kde_datadir)/quanta/tags
	$(INSTALL_DATA) tagdata.rc $(kde_datadir)/quanta/tagdata.rc
	$(INSTALL_DATA) ol.tag $(kde_datadir)/quanta/tags/ol.tag
	$(INSTALL_DATA) body.tag $(kde_datadir)/quanta/tags/body.tag
	$(INSTALL_DATA) base.tag $(kde_datadir)/quanta/tags/base.tag
	$(INSTALL_DATA) frameset.tag $(kde_datadir)/quanta/tags/frameset.tag
	$(INSTALL_DATA) head.tag $(kde_datadir)/quanta/tags/head.tag
	$(INSTALL_DATA) html.tag $(kde_datadir)/quanta/tags/html.tag
	$(INSTALL_DATA) isindex.tag $(kde_datadir)/quanta/tags/isindex.tag
	$(INSTALL_DATA) link.tag $(kde_datadir)/quanta/tags/link.tag
	$(INSTALL_DATA) meta.tag $(kde_datadir)/quanta/tags/meta.tag
	$(INSTALL_DATA) script.tag $(kde_datadir)/quanta/tags/script.tag
	$(INSTALL_DATA) style.tag $(kde_datadir)/quanta/tags/style.tag

uninstall-local:
	-rm -f $(kde_datadir)/quanta/tagdata.rc

####### kdevelop will overwrite this part!!! (end)############
