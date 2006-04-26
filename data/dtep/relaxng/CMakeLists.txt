kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES}  )


########### install files ###############

install_files(${QUANTA_DATA_INSTALL_DIR}/dtep/relaxng  FILES  anyName.tag attribute.tag choice.tag data.tag define.tag description.rc div.tag element.tag empty.tag except.tag externalRef.tag grammar.tag group.tag include.tag interleave.tag list.tag mixed.tag name.tag notAllowed.tag nsName.tag oneOrMore.tag optional.tag param.tag parentRef.tag ref.tag start.tag text.tag value.tag zeroOrMore.tag )

kde4_footer()



#original Makefile.am contents follow:

#
#METASOURCES = AUTO
#quanta_DATA =  anyName.tag attribute.tag choice.tag data.tag define.tag description.rc div.tag element.tag empty.tag except.tag externalRef.tag grammar.tag group.tag include.tag interleave.tag list.tag mixed.tag name.tag notAllowed.tag nsName.tag oneOrMore.tag optional.tag param.tag parentRef.tag ref.tag start.tag text.tag value.tag zeroOrMore.tag
#quantadir = ${quanta_datadir}/dtep/relaxng
