kde4_header()

add_subdirectory( quantaproject )
add_subdirectory( projecttree )

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES}  )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = quantaproject projecttree
#_SOURCES = PROJECTDOM.dtd
#
