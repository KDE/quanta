####### kdevelop will overwrite this part!!! (begin)##########


EXTRA_DIST = parser.cpp tag.h tag.cpp parser.h node.h node.cpp 

####### kdevelop will overwrite this part!!! (end)############

noinst_LIBRARIES = libparser.a

INCLUDES = $(all_includes)
DEFS = -I. -I$(srcdir) -I$(top_srcdir)
libparser_a_METASOURCES = AUTO
libparser_a_SOURCES = \
		node.cpp\
		parser.cpp\
		tag.cpp
