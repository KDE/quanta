####### kdevelop will overwrite this part!!! (begin)##########

INCLUDES = $(all_includes)

libhighlight_a_METASOURCES = AUTO

noinst_LIBRARIES = libhighlight.a

libhighlight_a_SOURCES = highlight.cpp 


EXTRA_DIST = highlight.cpp highlight.h 

####### kdevelop will overwrite this part!!! (end)############
highlight.moc : highlight.h
	$(MOC) ./highlight.h -o highlight.moc

highlight.cpp : highlight.moc

INCLUDES = $(all_includes)

libhighlight_a_METASOURCES = AUTO

noinst_LIBRARIES = libhighlight.a

libhighlight_a_SOURCES = highlight.cpp 

EXTRA_DIST = highlight.cpp highlight.h 
