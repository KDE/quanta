####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = 48x48 16x16 22x22 32x32

####### kdevelop will overwrite this part!!! (end)############

EXTRA_DIST = preview1.png preview2.png preview3.png preview4.png quantalogo.png

install-data-local:
	$(mkinstalldirs) $(kde_datadir)/quanta/toolbar/
	$(INSTALL_DATA) $(srcdir)/preview1.png $(kde_datadir)/quanta/toolbar/preview1.png
	$(INSTALL_DATA) $(srcdir)/preview2.png $(kde_datadir)/quanta/toolbar/preview2.png
	$(INSTALL_DATA) $(srcdir)/preview3.png $(kde_datadir)/quanta/toolbar/preview3.png
	$(INSTALL_DATA) $(srcdir)/preview4.png $(kde_datadir)/quanta/toolbar/preview4.png
	$(INSTALL_DATA) $(srcdir)/quantalogo.png $(kde_datadir)/quanta/toolbar/quantalogo.png

uninstall-local:
	-rm -f $(kde_datadir)/quanta/toolbar/preview1.png
	-rm -f $(kde_datadir)/quanta/toolbar/preview2.png
	-rm -f $(kde_datadir)/quanta/toolbar/preview3.png
	-rm -f $(kde_datadir)/quanta/toolbar/preview4.png
	-rm -f $(kde_datadir)/quanta/toolbar/quantalogo.png
