lib_LTLIBRARIES = libxsldbg.la

libxsldbg_la_LDFLAGS = -version-info $(XSLDBG_VERSION_INFO)

libxsldbg_la_METASOURCES = AUTO

libxsldbg_la_SOURCES = allmoc.cpp xsldbgthread.c xsldbg.c variable_cmds.c utils.c trace_cmds.c template_cmds.c search_cmds.c search.c param_cmds.c os_cmds.c options_unix.c options.c option_cmds.c nodeview_cmds.c help_unix.c files_unix.c files.c file_cmds.c debugXSL.c debug.c callstack.c breakpoint_cmds.c breakpoint.c arraylist.c

libxsldbg_la_LIBADD = $(LIBXSLT_LIBS)

INCLUDES = -I .. $(LIBXML_CFLAGS)  $(LIBXSLT_CFLAGS)  $(all_includes)
AM_CFLAGS = -DDOCS_PATH=\"$(DOCS_PATH)\"

