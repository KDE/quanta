####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = tagdata.rc 

install-data-local:
	$(mkinstalldirs) $(kde_datadir)/quanta/
	$(mkinstalldirs) $(kde_datadir)/quanta/doc
	$(mkinstalldirs) $(kde_datadir)/quanta/tags
	$(INSTALL_DATA) tagdata.rc $(kde_datadir)/quanta/tagdata.rc
	$(INSTALL_DATA) ol.tag $(kde_datadir)/quanta/tags/ol.tag
	$(INSTALL_DATA) body.tag $(kde_datadir)/quanta/tags/body.tag

uninstall-local:
	-rm -f $(kde_datadir)/quanta/tagdata.rc

####### kdevelop will overwrite this part!!! (end)############
