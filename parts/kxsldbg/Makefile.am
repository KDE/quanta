####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kxsldbg xsldbg

## INCLUDES were found outside kdevelop specific part

kxsldbg_SOURCES = kxsldbg.cpp main.cpp
kxsldbg_LDADD   = ./kxsldbgpart/libqtnotfier/libqtnotfier.la ./kxsldbgpart/libkxsldbgpart.la ./kxsldbgpart/libxsldbg/libxsldbg.la  $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) $(LIBSOCKET)

SUBDIRS = kxsldbgpart

EXTRA_DIST = main.cpp xsldbgmain.c simpleio.c kxsldbg.cpp kxsldbg.h kxsldbg.desktop kxsldbg_shell.rc

applnk_DATA = kxsldbg.desktop
applnkdir = $(kde_appsdir)/Development

xsldbg_SOURCES = \
	xsldbgmain.c  \
	simpleio.c

xsldbg_CFLAGS = $(XSLDBG_CFLAGS) $(all_includes)

xsldbg_DEPENDENCIES  = kxsldbgpart/libxsldbg/libxsldbg.la kxsldbgpart/libqtnotfier/libqtnotfier.la

# use of configure defined libraries
xsldbg_LDADD = kxsldbgpart/libxsldbg/libxsldbg.la  kxsldbgpart/libqtnotfier/libqtnotfier.la $(LIB_QT) $(LIBXSLT_LIBS) $(LIBXML_LIBS)

# the library search path.
xsldbg_LDFLAGS = $(all_libraries)

####### kdevelop will overwrite this part!!! (end)############
# These paths are KDE specific. Use them:
# kde_appsdir         Where your application's menu entry (.desktop) should go to.
# kde_icondir         Where your icon should go to - better use KDE_ICON.
# kde_sounddir        Where your sounds should go to.
# kde_htmldir         Where your docs should go to. (contains lang subdirs)
# kde_datadir         Where you install application data. (Use a subdir)
# kde_locale          Where translation files should go to. (contains lang subdirs)
# kde_cgidir          Where cgi-bin executables should go to.
# kde_confdir         Where config files should go to (system-wide ones with default values).
# kde_mimedir         Where mimetypes .desktop files should go to.
# kde_servicesdir     Where services .desktop files should go to.
# kde_servicetypesdir Where servicetypes .desktop files should go to.
# kde_toolbardir      Where general toolbar icons should go to (deprecated, use KDE_ICON).
# kde_wallpaperdir    Where general wallpapers should go to.
# kde_templatesdir    Where templates for the "New" menu (Konqueror/KDesktop) should go to.
# kde_bindir          Where executables should go to. Use bin_PROGRAMS or bin_SCRIPTS.
# kde_libdir          Where shared libraries should go to. Use lib_LTLIBRARIES.
# kde_moduledir       Where modules (e.g. parts) should go to. Use kde_module_LTLIBRARIES.
# kde_styledir        Where Qt/KDE widget styles should go to (new in KDE 3).
# kde_designerdir     Where Qt Designer plugins should go to (new in KDE 3).

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

METASOURCES = AUTO

# the application source, library search path, and link libraries
kxsldbg_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# this is where the desktop file will go
shelldesktopdir   = $(kde_appsdir)/Development
shelldesktop_DATA = kxsldbg.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/kxsldbg
shellrc_DATA = kxsldbg_shell.rc

# Uncomment the following two lines if you add a ui.rc file for your application to make use of
# KDE´s XML GUI builing
#rcdir = $(kde_datadir)/kxsldbg
#rc_DATA = kxsldbgui.rc

#WARNING: if you use a ui.rc file above, use:

# messages: rc.cpp

# instead of

# messages:

messages:
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/kxsldbg.pot; \
	fi

