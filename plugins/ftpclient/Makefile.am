####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = ftpclient.h ftpclient.cpp ftpclientview.cpp ftpclientview.h ftpclients.cpp ftpclients.h 

####### kdevelop will overwrite this part!!! (end)############

libdir = ${kde_libraries}/quanta/plugins
lib_LTLIBRARIES =libftpclient.la


libftpclient_la_LDFLAGS = -version-info 1:0

INCLUDES = $(all_includes)
DEFS = -I. -I$(srcdir) -I$(top_srcdir)
libftpclient_la_METASOURCES = AUTO
libftpclient_la_SOURCES = \
		ftpclients.cpp\
		ftpclient.cpp\
		ftpclientview.cpp
