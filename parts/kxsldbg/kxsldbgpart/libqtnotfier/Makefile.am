lib_LTLIBRARIES = libqtnotfier.la

libqtnotfier_la_LDFLAGS = -version-info $(XSLDBG_VERSION_INFO)
libqtnotfier_la_LIBADD = $(LIBXSLT_LIBS)
libqtnotfier_la_METASOURCES = AUTO

libqtnotfier_la_SOURCES = xsldbgthread.cpp xsldbgnotifier.cpp xsldbgevent.cpp xsldbgdebuggerbase.cpp qtnotifier2.cpp

INCLUDES = $(XSLDBG_CFLAGS) $(all_includes)
