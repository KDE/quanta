####### kdevelop will overwrite this part!!! (begin)##########


####### kdevelop will overwrite this part!!! (end)############

datadir = ${kde_datadir}/quanta/toolbar

data_DATA = quanta.png 

install-data-local:
	$(mkinstalldirs) $(kde_icondir)/hicolor/48x48/apps/
	$(INSTALL_DATA) $(srcdir)/quanta.png $(kde_icondir)/hicolor/48x48/apps/quanta.png

uninstall-local:
	-rm -f $(kde_icondir)/hicolor/48x48/apps/quanta.png
