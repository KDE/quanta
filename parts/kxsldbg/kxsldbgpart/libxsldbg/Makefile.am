####### kdevelop will overwrite this part!!! (begin)##########
lib_LTLIBRARIES = libxsldbg.la


## INCLUDES were found outside kdevelop specific part

libxsldbg_la_LDFLAGS = -version-info $(XSLDBG_VERSION_INFO)

libxsldbg_la_METASOURCES = AUTO

libxsldbg_la_SOURCES = allmoc.cpp xsldbgthread.c xsldbg.c variable_cmds.c utils.c trace_cmds.c template_cmds.c search_cmds.c search.c param_cmds.c os_cmds.c options_unix.c options.c option_cmds.c nodeview_cmds.c help_unix.c files_unix.c files.c file_cmds.c debugXSL.c debug.c callstack.c breakpoint_cmds.c breakpoint.c arraylist.c


EXTRA_DIST = arraylist.c breakpoint.c breakpoint_cmds.c callstack.c debug.c debugXSL.c file_cmds.c files.c files_unix.c help_unix.c nodeview_cmds.c option_cmds.c options.c options_unix.c os_cmds.c param_cmds.c search.c search_cmds.c template_cmds.c trace_cmds.c utils.c variable_cmds.c xsldbg.c xsldbgthread.c allmoc.cpp arraylist.h breakpoint.h callstack.h cmds.h debug.h debugXSL.h files.h help.h options.h qtnotifier2.h search.h utils.h xsldbg.h xsldbgconfig.h xsldbgdebuggerbase.h xsldbgevent.h xsldbgio.h xsldbgmsg.h xsldbgnotifier.h xsldbgthread.h

####### kdevelop will overwrite this part!!! (end)############

INCLUDES = -I .. $(LIBXML_CFLAGS)  $(LIBXSLT_CFLAGS)  $(all_includes)
AM_CFLAGS = -DDOCS_PATH=\"$(DOCS_PATH)\"

