####### kdevelop will overwrite this part!!! (begin)##########


####### kdevelop will overwrite this part!!! (end)############

datadir = ${kde_datadir}/quanta/toolbar

data_DATA = delete.png empty1x16.png java.png mini-book1.png mini-book2.png mini-modules.png quanta.png save_small.png tag_br_small.png tag_font_small.png tag_hr_small.png empty16x16.png 

install-data-local:
	$(mkinstalldirs) $(kde_icondir)/hicolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/quanta.png $(kde_icondir)/hicolor/16x16/apps/quanta.png

uninstall-local:
	-rm -f $(kde_icondir)/hicolor/16x16/apps/quanta.png

