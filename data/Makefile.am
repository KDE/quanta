
EXTRA_DIST = tagdata.rc 

install-data-local:
	$(mkinstalldirs) $(kde_datadir)/quanta/
	$(mkinstalldirs) $(kde_datadir)/quanta/doc
	$(mkinstalldirs) $(kde_datadir)/quanta/tags
	$(INSTALL_DATA) tagdata.rc $(kde_datadir)/quanta/tagdata.rc
	$(INSTALL_DATA) base.tag $(kde_datadir)/quanta/tags/base.tag
	$(INSTALL_DATA) body.tag $(kde_datadir)/quanta/tags/body.tag
	$(INSTALL_DATA) div.tag $(kde_datadir)/quanta/tags/div.tag
	$(INSTALL_DATA) frameset.tag $(kde_datadir)/quanta/tags/frameset.tag
	$(INSTALL_DATA) h1.tag $(kde_datadir)/quanta/tags/h1.tag
	$(INSTALL_DATA) h2.tag $(kde_datadir)/quanta/tags/h2.tag
	$(INSTALL_DATA) h3.tag $(kde_datadir)/quanta/tags/h3.tag
	$(INSTALL_DATA) h4.tag $(kde_datadir)/quanta/tags/h4.tag
	$(INSTALL_DATA) h5.tag $(kde_datadir)/quanta/tags/h5.tag
	$(INSTALL_DATA) h6.tag $(kde_datadir)/quanta/tags/h6.tag
	$(INSTALL_DATA) head.tag $(kde_datadir)/quanta/tags/head.tag
	$(INSTALL_DATA) hr.tag $(kde_datadir)/quanta/tags/hr.tag
	$(INSTALL_DATA) html.tag $(kde_datadir)/quanta/tags/html.tag
	$(INSTALL_DATA) isindex.tag $(kde_datadir)/quanta/tags/isindex.tag
	$(INSTALL_DATA) link.tag $(kde_datadir)/quanta/tags/link.tag
	$(INSTALL_DATA) li.tag $(kde_datadir)/quanta/tags/li.tag
	$(INSTALL_DATA) meta.tag $(kde_datadir)/quanta/tags/meta.tag
	$(INSTALL_DATA) ol.tag $(kde_datadir)/quanta/tags/ol.tag
	$(INSTALL_DATA) p.tag $(kde_datadir)/quanta/tags/p.tag
	$(INSTALL_DATA) script.tag $(kde_datadir)/quanta/tags/script.tag
	$(INSTALL_DATA) style.tag $(kde_datadir)/quanta/tags/style.tag
	$(INSTALL_DATA) ul.tag $(kde_datadir)/quanta/tags/ul.tag




uninstall-local:
	-rm -f $(kde_datadir)/quanta/tagdata.rc

