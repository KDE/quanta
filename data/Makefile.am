####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = doc tags icons templates toolbars 

EXTRA_DIST = actions.rc plugins.rc 

####### kdevelop will overwrite this part!!! (end)############
datadir = ${kde_datadir}/quanta

data_DATA = plugins.rc actions.rc chars

install-data-local:
	$(mkinstalldirs) $(prefix)/share/apps/kate/syntax/
	$(INSTALL_DATA) $(srcdir)/php.xml $(prefix)/share/apps/kate/syntax/php.xml
	$(mkinstalldirs) $(prefix)/share/apps/katepart/syntax/
	$(INSTALL_DATA) $(srcdir)/php.xml $(prefix)/share/apps/katepart/syntax/php.xml

	$(mkinstalldirs) $(prefix)/share/apps/kate/syntax/
	$(INSTALL_DATA) $(srcdir)/coldfusion.xml $(prefix)/share/apps/kate/syntax/coldfusion.xml
	$(mkinstalldirs) $(prefix)/share/apps/katepart/syntax/
	$(INSTALL_DATA) $(srcdir)/coldfusion.xml $(prefix)/share/apps/katepart/syntax/coldfusion.xml

uninstall-local:
	-rm -f $(prefix)/share/apps/kate/syntax/php.xml
	-rm -f $(prefix)/share/apps/katepart/syntax/php.xml
	-rm -f $(prefix)/share/apps/kate/syntax/coldfusion.xml
	-rm -f $(prefix)/share/apps/katepart/syntax/coldfusion.xml
