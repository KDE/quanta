####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = quanta
quanta_SOURCES = qextfileinfo.cpp quantaview_slots.cpp document.cpp quantaview_init.cpp quanta_init.cpp quantaview.cpp quantadoc.cpp quanta.cpp main.cpp 
quanta_LDADD   = ./widgets/libwidgets.a ./kwrite/libkwrite.a ./treeviews/libtreeviews.a ./kwrite/highlight/libhighlight.a ./tagdialogs/libtagdialogs.a ./dialogs/libdialogs.a ./parser/libparser.a ./project/libproject.a -lkparts -lkhtml -lkjs -lktexteditor -lkspell -lkfile -lkdeui -lkdecore -lqt -lXext -lX11 $(LIBSOCKET)

SUBDIRS = icons widgets kwrite treeviews tagdialogs data dialogs parser project 

EXTRA_DIST = main.cpp quanta.cpp quanta.h quantadoc.cpp quantadoc.h quantaview.cpp quantaview.h resource.h quanta.desktop quanta.xpm mini-quanta.xpm quanta_init.cpp quantaview_init.cpp document.cpp document.h quantaview_slots.cpp qextfileinfo.cpp qextfileinfo.h 

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) quanta.desktop $(kde_appsdir)/Applications/quanta.desktop
	$(mkinstalldirs) $(kde_icondir)/locolor/32x32/apps/
	$(INSTALL_DATA) quanta.xpm $(kde_icondir)/locolor/32x32/apps/quanta.xpm
	$(mkinstalldirs) $(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) mini-quanta.xpm $(kde_icondir)/locolor/16x16/apps/quanta.xpm

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/quanta.desktop
	-rm -f $(kde_icondir)/locolor/32x32/apps/quanta.xpm
	-rm -f $(kde_icondir)/locolor/16x16/apps/quanta.xpm

####### kdevelop will overwrite this part!!! (end)############
# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

quanta_METASOURCES = AUTO

# the library search path. 
quanta_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(quanta_METASOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) -C -ki18n -x $(kde_includes)/kde.pot $$LIST -o ../po/quanta.pot; \
	fi

