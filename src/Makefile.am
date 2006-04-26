INCLUDES = -I$(top_srcdir)/lib -I$(kde_includes)/kinterfacedesigner \
	-I$(kde_includes)/kdevelop/shell/profileengine -I$(kde_includes)/kdevelop/interfaces -I$(kde_includes)/kdevelop/shell \
	$(all_includes)
METASOURCES = AUTO
bin_PROGRAMS = kuanta
kuanta_LDFLAGS = -avoid-version -L$(top_builddir)/lib $(all_libraries)
kuanta_SOURCES = main.cpp kdevquantaextension.cpp
noinst_HEADERS = kdevquantaextension.h
kuanta_LDADD = -lkdevquanta -lkdevshell

rcdir = $(kde_datadir)/kuanta

rc_DATA = kuantarc kuantaui.rc

xdg_apps_DATA = kuanta.desktop

servicedir = $(kde_servicesdir)

servicetypedir = $(kde_servicetypesdir)
servicetype_DATA = kdevelopquanta.desktop

profiledir = $(kde_datadir)/kdevelop/profiles/quanta
profile_DATA = profile.config

kdemimedir = $(kde_mimedir)/application
kdemime_DATA = x-quanta.desktop
