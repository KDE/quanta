noinst_LTLIBRARIES = libproject.la
METASOURCES = AUTO

libproject_la_SOURCES = projectoptions.ui projectnewgenerals.ui \
			projectnewgeneral.cpp projectnewlocals.ui projectnewlocal.cpp rescanprj.cpp rescanprjdir.ui \
			projectnewfinals.ui projectuploads.ui projectnewfinal.cpp projectupload.cpp project.cpp \
			projectnewwebs.ui projectnewweb.cpp uploadprofiledlgs.ui projectlist.cpp \
			membereditdlgs.ui teammembersdlgs.ui teammembersdlg.cpp membereditdlg.cpp \
			subprojecteditdlgs.ui eventconfigurationdlgs.ui eventeditordlgs.ui eventconfigurationdlg.cpp \
			eventeditordlg.cpp uploadprofiles.cpp uploadprofilespage.ui projectprivate.cpp

AM_CPPFLAGS = -I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/quanta/parsers \
	-I$(top_srcdir)/quanta/dialogs \
	-I$(top_srcdir)/quanta/dialogs/tagdialogs \
	-I$(top_srcdir)/quanta/treeviews \
	-I$(top_srcdir)/quanta/src \
	-I$(top_srcdir)/quanta/utility \
	-I$(top_srcdir)/lib \
	-I$(top_srcdir)/quanta/components/debugger \
	-I$(top_srcdir)/quanta/components/debugger/interfaces \
	-I$(top_builddir)/quanta/dialogs \
	$(KMDI_INCLUDES) $(all_includes)

noinst_HEADERS = teammembersdlg.h membereditdlg.h eventconfigurationdlg.h \
	eventeditordlg.h
