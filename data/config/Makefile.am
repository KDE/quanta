# this is where the XML-GUI resource file goes
rcdir = ${quanta_datadir}

rc_DATA = plugins.rc actions.rc quantaui.rc

if QUANTAUIRC_HOOK
install-data-hook:
	mv $(DESTDIR)$(rcdir)/quantaui.rc $(DESTDIR)$(rcdir)/$(package)ui.rc || true
else
install-data-hook:
#no hook
endif
kde_services_DATA = quanta_preview_config.desktop
