bin_PROGRAMS = quanta
quanta_SOURCES = qextfileinfo.cpp quantaview_slots.cpp document.cpp quantaview_init.cpp quanta_init.cpp quantaview.cpp quantadoc.cpp quanta.cpp main.cpp 
quanta_LDADD   = ./widgets/libwidgets.a ./kwrite/libkwrite.a ./treeviews/libtreeviews.a ./kwrite/highlight/libhighlight.a ./tagdialogs/libtagdialogs.a ./dialogs/libdialogs.a ./parser/libparser.a ./project/libproject.a ./toolbar/libtoolbar.a -lkparts -lkhtml -lkjs -lktexteditor -lkspell -lkfile -lkdeui -lkdecore -lqt -lXext -lX11 $(LIBSOCKET)

SUBDIRS = icons widgets kwrite treeviews tagdialogs data dialogs parser project toolbar 

EXTRA_DIST = main.cpp quanta.cpp quanta.h quantadoc.cpp quantadoc.h quantaview.cpp quantaview.h resource.h quanta.desktop quanta.xpm mini-quanta.xpm quanta_init.cpp quantaview_init.cpp document.cpp document.h quantaview_slots.cpp qextfileinfo.cpp qextfileinfo.h 

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/quanta.desktop $(kde_appsdir)/Applications/quanta.desktop

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/quanta.desktop

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

