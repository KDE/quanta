####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = doc tags icons templates toolbars 

EXTRA_DIST = actions.rc php.xml 

install-data-local:
	$(mkinstalldirs) $(prefix)/share/apps/kate/syntax/
	$(INSTALL_DATA) $(srcdir)/php.xml $(prefix)/share/apps/kate/syntax/php.xml

uninstall-local:
	-rm -f $(prefix)/share/apps/kate/syntax/php.xml

####### kdevelop will overwrite this part!!! (end)############
datadir = ${kde_datadir}/quanta

data_DATA = actions.rc chars
