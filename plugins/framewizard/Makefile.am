bin_PROGRAMS = framewizard
framewizard_METASOURCES = AUTO

framewizard_SOURCES = areaattributedb.cpp areaattributedb.h fmfpeditor.ui fmrceditor.ui framewizard.ui selectablearea.cpp selectablearea.h treenode.cpp treenode.h visualframeeditor.cpp visualframeeditor.h main.cpp

INCLUDES = -I$(top_srcdir)/quanta/plugins/framewizard -I$(top_builddir)/quanta/plugins/framewizard $(all_includes)

framewizard_LDADD = $(LIB_KDECORE) $(LIB_KDEUI)  $(LIB_KFILE) 
framewizard_LDFLAGS = $(all_libraries)
