####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = keysconfig.png filesmask.png previewoptions.png tagstyle.png 

install-data-local:
	$(mkinstalldirs) $(kde_datadir)/quanta/toolbar/
	$(INSTALL_DATA) keysconfig.png $(kde_datadir)/quanta/toolbar/keysconfig.png
	$(mkinstalldirs) $(kde_datadir)/quanta/toolbar/
	$(INSTALL_DATA) filesmask.png $(kde_datadir)/quanta/toolbar/filesmask.png
	$(mkinstalldirs) $(kde_datadir)/quanta/toolbar/
	$(INSTALL_DATA) previewoptions.png $(kde_datadir)/quanta/toolbar/previewoptions.png
	$(mkinstalldirs) $(kde_datadir)/quanta/toolbar/
	$(INSTALL_DATA) tagstyle.png $(kde_datadir)/quanta/toolbar/tagstyle.png

uninstall-local:
	-rm -f $(kde_datadir)/quanta/toolbar/keysconfig.png
	-rm -f $(kde_datadir)/quanta/toolbar/filesmask.png
	-rm -f $(kde_datadir)/quanta/toolbar/previewoptions.png
	-rm -f $(kde_datadir)/quanta/toolbar/tagstyle.png

####### kdevelop will overwrite this part!!! (end)############

EXTRA_DIST = quanta.png

install-data-local:
	$(mkinstalldirs) $(kde_icondir)/hicolor/48x48/apps/
	$(INSTALL_DATA) $(srcdir)/quanta.png $(kde_icondir)/hicolor/48x48/apps/quanta.png

uninstall-local:
	-rm -f $(kde_icondir)/hicolor/48x48/apps/quanta.png
