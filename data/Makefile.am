EXTRA_DIST = tagdata.rc toolbars.rc

install-data-local:
	$(mkinstalldirs) $(kde_datadir)/quanta/
	$(mkinstalldirs) $(kde_datadir)/quanta/doc
	$(mkinstalldirs) $(kde_datadir)/quanta/tags
	$(mkinstalldirs) $(kde_datadir)/quanta/doc/quanta/
	$(INSTALL_DATA) $(srcdir)/toolbars.rc $(kde_datadir)/quanta/toolbars.rc	
	$(INSTALL_DATA) $(srcdir)/tagdata.rc $(kde_datadir)/quanta/tagdata.rc
	$(INSTALL_DATA) $(srcdir)/a.tag $(kde_datadir)/quanta/tags/a.tag
	$(INSTALL_DATA) $(srcdir)/base.tag $(kde_datadir)/quanta/tags/base.tag
	$(INSTALL_DATA) $(srcdir)/basefont.tag $(kde_datadir)/quanta/tags/basefont.tag
	$(INSTALL_DATA) $(srcdir)/big.tag $(kde_datadir)/quanta/tags/big.tag
	$(INSTALL_DATA) $(srcdir)/blockquote.tag $(kde_datadir)/quanta/tags/blockquote.tag
	$(INSTALL_DATA) $(srcdir)/body.tag $(kde_datadir)/quanta/tags/body.tag
	$(INSTALL_DATA) $(srcdir)/br.tag $(kde_datadir)/quanta/tags/br.tag
	$(INSTALL_DATA) $(srcdir)/div.tag $(kde_datadir)/quanta/tags/div.tag
	$(INSTALL_DATA) $(srcdir)/frame.tag $(kde_datadir)/quanta/tags/frame.tag
	$(INSTALL_DATA) $(srcdir)/frameset.tag $(kde_datadir)/quanta/tags/frameset.tag
	$(INSTALL_DATA) $(srcdir)/h1.tag $(kde_datadir)/quanta/tags/h1.tag
	$(INSTALL_DATA) $(srcdir)/h2.tag $(kde_datadir)/quanta/tags/h2.tag
	$(INSTALL_DATA) $(srcdir)/h3.tag $(kde_datadir)/quanta/tags/h3.tag
	$(INSTALL_DATA) $(srcdir)/h4.tag $(kde_datadir)/quanta/tags/h4.tag
	$(INSTALL_DATA) $(srcdir)/h5.tag $(kde_datadir)/quanta/tags/h5.tag
	$(INSTALL_DATA) $(srcdir)/h6.tag $(kde_datadir)/quanta/tags/h6.tag
	$(INSTALL_DATA) $(srcdir)/head.tag $(kde_datadir)/quanta/tags/head.tag
	$(INSTALL_DATA) $(srcdir)/hr.tag $(kde_datadir)/quanta/tags/hr.tag
	$(INSTALL_DATA) $(srcdir)/html.tag $(kde_datadir)/quanta/tags/html.tag
	$(INSTALL_DATA) $(srcdir)/input.tag $(kde_datadir)/quanta/tags/input.tag
	$(INSTALL_DATA) $(srcdir)/isindex.tag $(kde_datadir)/quanta/tags/isindex.tag
	$(INSTALL_DATA) $(srcdir)/link.tag $(kde_datadir)/quanta/tags/link.tag
	$(INSTALL_DATA) $(srcdir)/li.tag $(kde_datadir)/quanta/tags/li.tag
	$(INSTALL_DATA) $(srcdir)/meta.tag $(kde_datadir)/quanta/tags/meta.tag
	$(INSTALL_DATA) $(srcdir)/ol.tag $(kde_datadir)/quanta/tags/ol.tag
	$(INSTALL_DATA) $(srcdir)/option.tag $(kde_datadir)/quanta/tags/option.tag
	$(INSTALL_DATA) $(srcdir)/p.tag $(kde_datadir)/quanta/tags/p.tag
	$(INSTALL_DATA) $(srcdir)/select.tag $(kde_datadir)/quanta/tags/select.tag
	$(INSTALL_DATA) $(srcdir)/script.tag $(kde_datadir)/quanta/tags/script.tag
	$(INSTALL_DATA) $(srcdir)/style.tag $(kde_datadir)/quanta/tags/style.tag
	$(INSTALL_DATA) $(srcdir)/table.tag $(kde_datadir)/quanta/tags/table.tag
	$(INSTALL_DATA) $(srcdir)/tbody.tag $(kde_datadir)/quanta/tags/tbody.tag
	$(INSTALL_DATA) $(srcdir)/td.tag $(kde_datadir)/quanta/tags/td.tag
	$(INSTALL_DATA) $(srcdir)/textarea.tag $(kde_datadir)/quanta/tags/textarea.tag
	$(INSTALL_DATA) $(srcdir)/th.tag $(kde_datadir)/quanta/tags/th.tag
	$(INSTALL_DATA) $(srcdir)/thead.tag $(kde_datadir)/quanta/tags/thead.tag
	$(INSTALL_DATA) $(srcdir)/tr.tag $(kde_datadir)/quanta/tags/tr.tag
	$(INSTALL_DATA) $(srcdir)/ul.tag $(kde_datadir)/quanta/tags/ul.tag
	$(INSTALL_DATA) $(srcdir)/documentation.html $(kde_datadir)/quanta/doc/documentation.html
	$(INSTALL_DATA) $(srcdir)/head_bg.gif $(kde_datadir)/quanta/doc/head_bg.gif
	$(INSTALL_DATA) $(srcdir)/minilogo.jpg $(kde_datadir)/quanta/doc/minilogo.jpg
	$(INSTALL_DATA) $(srcdir)/quanta-dialog-xml.html $(kde_datadir)/quanta/doc/quanta/quanta-dialog-xml.html
	$(INSTALL_DATA) $(srcdir)/quanta.docrc $(kde_datadir)/quanta/doc/quanta.docrc

uninstall-local:
	-rm -f  $(kde_datadir)/quanta/tagdata.rc
	-rm -fr $(kde_datadir)/quanta/doc/*
	